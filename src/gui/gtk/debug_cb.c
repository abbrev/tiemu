#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <gtk/gtk.h>
#include <ctype.h>

#include "debug_cb.h"
#include "debug_dbox.h"
#include "support.h"

#include "interface.h"
#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "paths.h"
#include "support.h"
#include "utils.h"
#include "pgmentbkpt_dbox.h"
#include "pgmentbkpt_cb.h"
#include "trap_dbox.h"
#include "trap_cb.h"
#include "dbgopt_dbox.h"
#include "dbgopt_cb.h"
#include "msg_cb.h"
#include "dbkpt_dbox.h"
#include "dbkpt_cb.h"
#include "bkpt_cause.h"
#include "main.h"

#include "sysdeps.h"

#define DEBUG_LINES 20

/* Used widget */
static GtkWidget *debugger_dbox = NULL;
static GtkWidget *code_clist;
static GtkWidget *mem_clist;
static GtkWidget *stack_clist;
static GtkWidget *reg_text;
static GtkWidget *statusbar;

gint refresh_register_dbox(void);
gint refresh_code_dbox(void);
gint refresh_stack_dbox(void);
gint refresh_memory_dbox(void);

static void
setup_clist(GtkWidget *clist, gchar *title[], int n)
{
  /* The ONLY POSSIBLE VALUES of n are 2 and 3 */
  GtkListStore *list;
  GtkTreeModel *model;
  int i;

  if (n == 3)
    list = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING,
			      G_TYPE_STRING);
  else
    list = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
  model = GTK_TREE_MODEL(list);
  
  gtk_tree_view_set_model(GTK_TREE_VIEW(clist), model); 
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(clist), TRUE); 
  
  for (i = 0; i < n; i++)
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), i, title[i],
						gtk_cell_renderer_text_new(),
						"text", 0, NULL);
  gtk_list_store_clear(list);
}

gint display_debugger_dbox(void)
{
  GtkWidget *dbox;
  GtkTreeSelection *sel;
    
  gchar *t_code[3] = { _("B"), _("Adr"), _("Code") };
  gchar *t_mem[3] = { _("Addr"), _("Dump"), _("Char") };
  gchar *t_stack[2] = { _("SP"), _("Value") };
  
  if(debugger_dbox == NULL)
    {
      dbox = create_debugger_dbox();
      debugger_dbox = dbox;

      reg_text = lookup_widget(dbox, "text5");
      
      code_clist = lookup_widget(dbox, "clist1");
      setup_clist(code_clist, t_code, 3);
      mem_clist = lookup_widget(dbox, "clist2");
      setup_clist(mem_clist, t_mem, 3);
      stack_clist = lookup_widget(dbox, "clist3");
      setup_clist(stack_clist, t_stack, 2);

      statusbar = lookup_widget(dbox, "statusbar1");

      sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(code_clist));
      gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);
      
      gtk_widget_show_all(dbox);

      g_signal_connect(G_OBJECT(sel), "changed",
		       G_CALLBACK(on_code_clist_selection_changed), NULL);

      display_bkpt_cause();
    }

  refresh_stack_dbox();
  refresh_memory_dbox();
  refresh_code_dbox();
  refresh_register_dbox();
      
  return 0;
}


/* Local persistant variables */
GtkWidget *data_bkpt_dbox = NULL;
gint bkpt_encountered = 0;
gint data_addr = 0x000000;
gint selected_row = -1;

/* Some linked-list for breakpoints */
GList *bkpt_address_list = NULL;
GList *bkpt_access_list = NULL;
GList *bkpt_vector_list = NULL;
GList *bkpt_trap_list = NULL;


/*****************/
/* Debugger dbox */
/*****************/

static int put_in_sb(gchar *s)
{
  gint id;

  id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), s);
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), id, s);

  return id;
}


/*
  Called when the Debugger Dbox is destroyed 
*/
void
on_debugger_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  gint i;
  CODE_BKPT *s;

  // The debugger dbox is not active any longer
  code_clist = NULL;
  debugger_dbox = NULL;

  // Restore breakpoints
  // Bkpts are removed when the debugger is launched so
  // that we can 'single step'
  for(i=0; i<g_list_length(bkpt_address_list); i++)
    {
      s = (CODE_BKPT *)g_list_nth_data(bkpt_address_list, i);
      
      ti68k_setBreakpointAddress(s->address);
    }
  
  // Emulation core in free running
  unhalt();
}


/*
  Initialize the 'Registers' box
*/
gint refresh_register_dbox(void)
{
  GtkWidget *text = reg_text;
  GtkTextBuffer *buf;
  GtkTextIter start, end;
  GtkTextTag *red_tag;
  gint i;
  gchar buffer[MAXCHARS];
  static gint prev_Dx[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  static gint prev_Ax[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  static gint prev_SP = 0;
  static gint prev_SR = 0;
  static gint prev_PC = 0;

  buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
  gtk_text_buffer_set_text(buf, "", -1);
  gtk_text_buffer_get_start_iter(buf, &start);
  gtk_text_buffer_get_end_iter(buf, &end);

  red_tag = gtk_text_buffer_create_tag(buf, "red_text", "foreground", "red", NULL);

  /* Display the Dx & Ax registers */
  for(i=0; i<8; i++)
    {
      sprintf(buffer, "D%i=%08X", i, ti68k_getDataRegister(i));
      gtk_text_buffer_insert(buf, &end, buffer, -1);
      if(prev_Dx[i] != ti68k_getDataRegister(i))
	gtk_text_buffer_apply_tag(buf, red_tag, &start, &end);
      
      start = end;

      sprintf(buffer, "  ");
      gtk_text_buffer_insert(buf, &end, buffer, -1);
      
      sprintf(buffer, "A%i=%08X\n", i, ti68k_getAddressRegister(i));
      gtk_text_buffer_insert(buf, &end, buffer, -1);
      if(prev_Ax[i] != ti68k_getAddressRegister(i))
	gtk_text_buffer_apply_tag(buf, red_tag, &start, &end);
      
      start = end;
    }

  gtk_text_buffer_insert(buf, &end, "\n", -1);
  start = end;
  
  /* Display the SP register */
  sprintf(buffer, "SP=%06X\n", ti68k_getSpRegister());
  gtk_text_buffer_insert(buf, &end, buffer, -1);
  if(prev_SP != ti68k_getSpRegister())
    gtk_text_buffer_apply_tag(buf, red_tag, &start, &end);

  start = end;

  /* Display the SR register */
  sprintf(buffer, "SR=%04X      ", ti68k_getSrRegister());
  gtk_text_buffer_insert(buf, &end, buffer, -1);
  if(prev_SR != ti68k_getSrRegister())
    gtk_text_buffer_apply_tag(buf, red_tag, &start, &end);

  start = end;

  /* Display the PC register */
  sprintf(buffer, "PC=%06X  \n", ti68k_getPcRegister());
  gtk_text_buffer_insert(buf, &end, buffer, -1);
  if(prev_PC != ti68k_getPcRegister())
    gtk_text_buffer_apply_tag(buf, red_tag, &start, &end);

  /* Display the status flag register */
  /*
  sprintf(buffer, "T=%i  S=%i  M=%i  I=%i  \n", 0, 0, 0, 0);
  gtk_text_buffer_insert(buf, &end, buffer, -1);

  sprintf(buffer, "X=%i  N=%i  Z=%i  V=%i  C=%i\n", 0, 0, 0, 0, 0);
  gtk_text_buffer_insert(buf, &end, buffer, -1);

  */
  gtk_text_buffer_insert(buf, &end, ti68k_getFlagRegister(), -1);

  /* Store old values for colour display */
  for(i=0; i<8; i++)
    {
      prev_Dx[i] = ti68k_getDataRegister(i);
      prev_Ax[i] = ti68k_getAddressRegister(i);
    }
  prev_SR = ti68k_getSrRegister();
  prev_SP = ti68k_getSpRegister();
  prev_PC = ti68k_getPcRegister();

  return 0;
}

/*
  Initialize the 'code/disasm' box
*/

/* Utility function */
gint refresh_breakpoints(GtkWidget *widget)
{
#if 0 /* FUCKED */
  GtkWidget *clist = code_clist;
  GdkPixmap *pixmap_run;
  GdkBitmap *mask_run;
  GdkPixmap *pixmap_bkpt;
  GdkBitmap *mask_bkpt;
  GdkPixmap *pixmap_void;
  GdkBitmap *mask_void;
  gint i, j;
  CODE_BKPT *s1;
  gint addr1, addr2, addr;
  gint res;

  /* Load pixmaps */
  open_xpm("run.xpm", widget, &pixmap_run, &mask_run);
  open_xpm("bkpt.xpm", widget, &pixmap_bkpt, &mask_bkpt);
  open_xpm("void.xpm", widget, &pixmap_void, &mask_void);

  /* Erase pixmaps entries */
  for(j=0; j<DEBUG_LINES; j++)
    {
      gtk_clist_set_pixmap((GtkCList *)clist, j, 0, 
			   pixmap_void, mask_void);
    }
 
  /* Fill the clist */
  //printf("list length: %i, %p\n", g_list_length(bkpt_address_list), bkpt_address_list);
  for(i=0; i<g_list_length(bkpt_address_list); i++)
    {
      s1 = (CODE_BKPT *)g_list_nth_data(bkpt_address_list, i);
      addr1 = s1->address;
      //printf("Breakpoint to localize at 0x%06x\n", addr1);

      for(j=0; j<DEBUG_LINES; j++)
	{
	  addr2 = GPOINTER_TO_INT(gtk_clist_get_row_data((GtkCList *)clist, 
							 j));
	  //printf("Address localized: 0x%06x\n", addr2);

	  if(addr1 == addr2)
	    {
	      //printf("Bkpt localized: 0x%06x at clist line %i\n", addr2, j);
	      gtk_clist_set_pixmap((GtkCList *)clist, j, 0, 
				   pixmap_bkpt, mask_bkpt);
	    }
	  //fprintf(stderr, "<%06X %06X>\n", addr2, getPcRegister());
	}
    }

  /* Update the run arrow */
  addr = ti68k_getPcRegister();
  //fprintf(stdout, "PC in refresh: %06X\n", ti68k_getPcRegister());
  res = gtk_clist_find_row_from_data((GtkCList *)clist, GINT_TO_POINTER(addr));
  //fprintf(stdout, "row in refresh: %i\n", res);
  if(res != -1)
    { // row found
      gtk_clist_set_pixmap((GtkCList *)clist, res, 0, pixmap_run, mask_run);
    }
  printf("\n");

  gdk_pixmap_unref(pixmap_run);
  gdk_pixmap_unref(pixmap_bkpt);
  gdk_pixmap_unref(pixmap_void);
#endif /* 0 */

  return 0;
}

gint refresh_code_dbox(void)
{
#if 0 /* FUCKED */
  GtkWidget *clist = code_clist;
  gchar *text[3];
  gint i, k;
  gint addr = ti68k_getPcRegister();
  gint offset;
  gchar buffer[MAXCHARS];
  gint row;

  //fprintf(stderr, "PC: %06X\n", getPcRegister());
  row = gtk_clist_find_row_from_data((GtkCList *)clist, 
				     GINT_TO_POINTER(ti68k_getPcRegister()));
  //fprintf(stdout, "row: %i\n", row);
  if(row != -1)
    { // row not found: address is outside, reload disasm
      refresh_breakpoints(debugger_dbox);
      return -1;
    }

  /* Fill the clist */

  for(i=0; i<options.code_lines; i++)
    gtk_clist_remove((GtkCList *)clist, 0);
  for(i=0; i<options.code_lines; i++)
    {
      offset = ti68k_disasm(addr, buffer);
      text[1] = g_strdup(buffer);
      text[1][9] = '\0';
      text[2] = g_strdup(buffer+10);
 
      gtk_clist_append((GtkCList *)clist, row_text);
      gtk_clist_set_row_data((GtkCList *)clist, i, 
			     GINT_TO_POINTER(addr)); // add addr to clist data
      //fprintf(stderr, "%i: %06X\n", i, addr);

      addr += offset;

      for(k=0; k<3; k++)
	{
	  g_free(text[k]);
	} 
    }

  /* Refresh breakpoints */
  //printf("clist pointer: %p\n", clist);
  refresh_breakpoints(debugger_dbox);
#endif /* 0 */

  return 0;
}

/*
  Initialize the 'memory' box
*/
gint refresh_memory_dbox(void)
{
#if 0 /* FUCKED */
  GtkWidget *clist = mem_clist;
  gchar *text[3];
  gint i, j, k;
  UBYTE *ti_ram = (UBYTE *)ti68k_getRamPtr();
  gint addr = data_addr;

  /* Fill the clist */
  for(i=0; i<options.mem_lines; i++)
    gtk_clist_remove((GtkCList *)clist, 0);
  for(j=0; j<options.mem_lines; j++, addr+=8)
    {
      text[0] = g_strdup_printf("%06X", addr);
      text[1] = g_strdup_printf("%02X %02X %02X %02X %02X %02X %02X %02X ",
				ti_ram[addr+0], ti_ram[addr+1],
				ti_ram[addr+2], ti_ram[addr+3],
				ti_ram[addr+4], ti_ram[addr+5],
				ti_ram[addr+6], ti_ram[addr+7]);
      
      text[2] = g_strdup_printf("%c%c%c%c%c%c%c%c", 
				isprint(ti_ram[addr+0]) ? ti_ram[addr+0] : '.',
				isprint(ti_ram[addr+1]) ? ti_ram[addr+1] : '.',
				isprint(ti_ram[addr+2]) ? ti_ram[addr+2] : '.',
				isprint(ti_ram[addr+3]) ? ti_ram[addr+3] : '.',
				isprint(ti_ram[addr+4]) ? ti_ram[addr+4] : '.',
				isprint(ti_ram[addr+5]) ? ti_ram[addr+5] : '.',
				isprint(ti_ram[addr+6]) ? ti_ram[addr+6] : '.',
				isprint(ti_ram[addr+7]) ? ti_ram[addr+7] : '.');
      
      gtk_clist_append((GtkCList *)clist, row_text);

      for(k=0; k<3; k++)
	{
	  //fprintf(stderr, "<%i %p %s>\n", k, row_text[k], row_text[k]); // bug here !!!
	  g_free(text[k]);
	}
    }
#endif /* 0 */

  return 0;
}

/*
  Initialize the 'stack' box
*/
gint refresh_stack_dbox(void)
{
#if 0 /* FUCKED */
  GtkWidget *clist = stack_clist;
  gchar *text[2];
  gint i, k;
  gint sp = ti68k_getSpRegister();
  UWORD *ti_ram = (UWORD *)ti68k_getRamPtr();
  gint addr;

  /* Fill the clist */
  for(i=0; i<options.stack_lines; i++)
    gtk_clist_remove((GtkCList *)clist, 0);
  for(i=0, addr = sp; i<10; i++, addr += 2)
    {
      text[0] = g_strdup_printf("%06X:", addr);
      text[1] = g_strdup_printf("%04X", ti_ram[addr]);
      
      gtk_clist_append((GtkCList *)clist, row_text);
    
      for(k=0; k<2; k++)
	{
	  g_free(text[k]);
	}
    }
#endif /* 0 */

  return 0;
}


/********************************/
/* Debugger dbox menu callbacks */
/********************************/

/*
  Debug|Run (F5)
*/
void
on_run1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  put_in_sb(_("Running..."));

  // Run the emulation core
  unhalt();
}


/*
  Debug|Step (F7)
*/

void
on_step1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gint addr;

  put_in_sb(_("One step (F7)"));
  DISPLAY("Step (F7)\n");

  // Set up an internal trap (DBTRACE) which will launch/refresh the
  // debugger when encountered
  addr = ti68k_getPcRegister();
  DISPLAY("addr=$%06x\n", addr);
  ti68k_doSingleStep(); // set trap
  unhalt();    // emulator in free running
}


void
on_step_over1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  //gint addr;
  
  put_in_sb(_("Step over."));
  fprintf(stderr, "Step over (F8): not yet implemented\n");

  /* Execute one instruction */
  //bkpt_encountered = !doInstructions(1, 0);

  refresh_code_dbox();

  /* Refres_breakpoints */
  refresh_breakpoints(code_clist);
}


/*
  Run up to cursor (F4)
*/
void
on_run_to_cursor1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gint addr_to_go, next_addr;
  gint i;
  gchar buffer[64];

  put_in_sb(_("Ran up to cursor."));
  fprintf(stderr, "Run to cursor (F4)\n");

  /* Execute some instructions */
  if(selected_row != -1)
    {
      printf("PC: 0x%06x\n", ti68k_getPcRegister());
#if 0 /* FUCKED */
      addr_to_go = GPOINTER_TO_INT(gtk_clist_get_row_data((GtkCList *)code_clist, selected_row));
#endif /* 0 */
      printf("addr to go: 0x%06x\n", addr_to_go);
      for(i=1, next_addr = ti68k_getPcRegister(); next_addr < addr_to_go; i++)
	{
	  next_addr += ti68k_disasm(next_addr, buffer);
	  printf("-> buffer: <%s>\n", buffer);
	  if(i > options.code_lines)
	    break;
	}
      //bkpt_encountered = !doInstructions(i, 0);
    }
  else
    return; 

  refresh_code_dbox();
  
  /* Refres_breakpoints */
  refresh_breakpoints(code_clist);
}


/*
  Debug|Break (F11)
*/
void
on_break1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gint i;
  gint addr;
  //GtkWidget *clist = code_clist;
  CODE_BKPT *s;

  put_in_sb(_("Break !"));
  halt();
  fprintf(stderr, "Break (F11)\n");

  /* Place a breakpoint */
  addr = ti68k_getPcRegister();
  //addr = getPcRegister() + disasm(getPcRegister(), buffer);
  i = ti68k_setBreakpointAddress(addr);

  s = (CODE_BKPT *)g_malloc(sizeof(CODE_BKPT));
  s->address = addr;
  s->id = i;
  bkpt_address_list = g_list_append(bkpt_address_list, s);

  refresh_code_dbox();
  
  /* Refres_breakpoints */
  refresh_breakpoints(code_clist);
}


/* Quit the 'Debugger' window */
void
on_exit2_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_widget_destroy(debugger_dbox);
}


void
on_set_breakpoint_at_selection1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *clist = code_clist;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  gint addr;
  gint i;
  CODE_BKPT *s, *s1;

  put_in_sb(_("Breakpoint added."));
  fprintf(stderr, "Breakpoints|Set bkpt at selection (F2)\n");
  /* Retrieve the selected line and its address */
  if(selected_row != -1)
    {
#if 0 /* FUCKED */
      addr = GPOINTER_TO_INT(gtk_clist_get_row_data((GtkCList *)clist, 
						    selected_row));
#endif /* 0 */
      /* Check whether we already have a breakpoint */      
      for(i=0; i<g_list_length(bkpt_address_list); i++)
	{
	  s1 = (CODE_BKPT *)g_list_nth_data(bkpt_address_list, i);
	  if(addr == s1->address)
	    {
	      /* If yes, then remove it (toggle bkpt) */
	      ti68k_delBreakpointAddress(s1->id);
	      bkpt_address_list = g_list_remove(bkpt_address_list, s1);
	      g_free(s1);
	      printf("Bkpt toggled\n");
	      refresh_breakpoints(code_clist);

	      return;
	    }
	}
    }
  else
    {
      addr = ti68k_getPcRegister();
    }
    
  /* Place a breakpoint */
  fprintf(stdout, "Place a breakpoint at %06X\n", addr);
  i = ti68k_setBreakpointAddress(addr);
  
  s = (CODE_BKPT *)g_malloc(sizeof(CODE_BKPT));
  s->address = addr;
  s->id = i;
  bkpt_address_list = g_list_append(bkpt_address_list, s);
  
  open_xpm("bkpt.xpm", code_clist, &pixmap, &mask);
#if 0 /* FUCKED */
  gtk_clist_set_pixmap((GtkCList *)clist, selected_row, 0, pixmap, mask);
#endif /* 0 */
  gdk_pixmap_unref(pixmap);
}


void
on_set_breakpoint_on_program_entry1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_pgmentrybkpt_dbox ();
}


void
on_set_breakpoint_on_trap_exception1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_trap_dbox();
}


void
on_set_data_breakpoint1_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  //gtk_widget_show(create_add_data_bkpt_dbox());
  display_data_bkpts_dbox();
}

/* Local functions used below */
static void clear_all_bkpt_address(gpointer data, gpointer user_data)
{
  CODE_BKPT *s = (CODE_BKPT *)data;
  ti68k_delBreakpointAddress(s->id);

  g_free(s);
}

void
on_clear_all_breakpoints1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  fprintf(stderr, "Breakpoints|Clear all bkpts\n");
  g_list_foreach(bkpt_address_list, clear_all_bkpt_address, NULL);
  g_list_free(bkpt_address_list);
  bkpt_address_list = NULL; // free ne met pas le ptr a null: bug !

  /* Refresh breakpoints */
  refresh_breakpoints(code_clist);
}


void
on_log1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  
}


void
on_view_log1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  msg_box2(_("Information"), 
	   _("This function is not available yet."));
}


void
on_edit_logged_addresses1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  msg_box2(_("Information"), 
	  _("This function is not available yet."));
}


void
on_log_link_port_send1_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  // to do
}


void
on_log_link_port_receive1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  // to do
}


void
on_clear_all1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  msg_box2(_("Information"), 
	   _("This function is not available yet."));
}


void
on_enable_logging1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  // to do
}


gboolean
on_clist1_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  gint row, column;
  GdkEventButton *bevent;

#if 0 /* FUCKED */
  if (!gtk_clist_get_selection_info (GTK_CLIST (widget), 
				     event->x, event->y, 
				     &row, &column)) 
    return FALSE;
#endif /* 0 */

  switch(event->type)
    {
    case GDK_BUTTON_PRESS:
      if(event->button==3)
	{
	  bevent=(GdkEventButton *)(event);
	  gtk_menu_popup(GTK_MENU(create_code_popup_menu()), 
			 NULL, NULL, NULL, NULL, 
			 bevent->button, bevent->time);

	  return TRUE;
	}
      break;
    case GDK_2BUTTON_PRESS:
      break;
    default:
      break;        
    }
  
  return FALSE;
}


void 
on_code_clist_selection_changed (GtkTreeSelection *sel, 
				 gpointer user_data)
{
  GList *paths;
  GtkTreeModel *model;

  paths = gtk_tree_selection_get_selected_rows(sel, &model);

  if (paths != NULL)
    selected_row = *gtk_tree_path_get_indices(g_list_nth_data(paths, 0));
  else
    selected_row = -1;

  g_list_foreach(paths, (GFunc)gtk_tree_path_free, NULL);
  g_list_free(paths);
}


gboolean
on_clist2_key_press_event              (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
  //gint row, column;

  printf("Key pressed\n");
  /*
  if (gtk_clist_get_selection_info (GTK_CLIST (widget), event->x, event->y, 
  &row, &column))
    {
      printf("at row %i, column %i\n", row, column);
    }
  */
  return FALSE;
}


gboolean
on_clist2_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  gint row, column;
  GdkEventButton *bevent;

#if 0 /* FUCKED */
  if (!gtk_clist_get_selection_info (GTK_CLIST (widget), 
				     event->x, event->y, 
				     &row, &column)) 
    return FALSE;
#endif /* 0 */

  switch(event->type)
    {
    case GDK_BUTTON_PRESS:
      if(event->button==3)
	{
	  bevent=(GdkEventButton *)(event);
	  gtk_menu_popup(GTK_MENU(create_memory_popup_menu()), 
			 NULL, NULL, NULL, NULL, 
			 bevent->button, bevent->time);

	  return TRUE;
	}
      break;
    case GDK_2BUTTON_PRESS:
      break;
    default:
      break;        
    }
  
  return FALSE;
}

/*******************/
/* Code popup menu */
/*******************/
//begin

void
on_go_to_address1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_widget_show_all(create_gotocode_dbox());
}


void
on_go_to_pc1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


/* Right button menu in the code part of the Debugger Window */
void
on_set_breakpoint1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  // Equivalent to the menu
  on_set_breakpoint_at_selection1_activate(menuitem, code_clist);
}


void
on_set_pc_to_selection1_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gint addr;

  /* Retrieve the selected line and its address */
  if(selected_row != -1)
    {
#if 0 /* FUCKED */
      addr = GPOINTER_TO_INT(gtk_clist_get_row_data((GtkCList *)code_clist, 
						    selected_row));
#endif /* 0 */
      ti68k_setPcRegister(addr);
      refresh_register_dbox();
    }
}


void
on_force_refresh1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  refresh_code_dbox();
  
  /* Refres_breakpoints */
  refresh_breakpoints(code_clist);
}

/********************/
/* Memoy popup menu */
/********************/

void
on_edit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  
}


void
on_go_to1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_widget_show_all(create_gotodata_dbox());
}


/********************************/
/* 'Address to go' dialog boxes */
/********************************/

static gint code_addr_to_go = 0x000000;
static gint data_addr_to_go = 0x000000;

void
on_entry10_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;

  s = gtk_editable_get_chars(editable, 0, -1);
  if(sscanf(s, "0x%6x", &code_addr_to_go) < 1)
    code_addr_to_go =  0x000000;

  g_free(s);
}


void
on_button45_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  ti68k_setPcRegister(code_addr_to_go);
  refresh_register_dbox();

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "gotocode_dbox"));
}

/* 'Address mem to go' dialog box */

void
on_entry11_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;

  s = gtk_editable_get_chars(editable, 0, -1);
  if(sscanf(s, "0x%6x", &data_addr_to_go) < 1)
    data_addr_to_go =  0x000000;

  g_free(s);
}


void
on_button99_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  data_addr = data_addr_to_go;
  refresh_memory_dbox();

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "gotodata_dbox"));
}


void
on_options1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_dbgopt_dbox();
}

/* Memory box events for test purposes */
gboolean
on_clist2_event                        (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  /*
  GdkEventButton *bevent = (GdkEventButton *)event;
  int x, y;
  int row, column;

  if( (event->type >= 4) && (event->type <= 9))
    {
      x = bevent->x;
      y = bevent->y;
      gtk_clist_get_selection_info (GTK_CLIST(widget), x, y, &row, &column);
      fprintf(stdout, "event %i, x=%i, y=%i, row=%i, column=%i\n",
	      event->type, x, y, row, column);
    }
  */
  return FALSE;
}

void
on_gotocode_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  //unhalt();
}


void
on_gotodata_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  //unhalt();
}

/**/

