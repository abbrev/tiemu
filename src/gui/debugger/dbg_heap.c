 /* Hey EMACS -*- linux-c -*- */
/* $Id: dbg_pclog.c 718 2004-11-22 12:35:39Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Li�vin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "intl.h"
#include "paths.h"
#include "support.h"
#include "ti68k_int.h"
#include "struct.h"
#include "dbg_all.h"
#include "handles.h"

enum { 
	    COL_ID, COL_ADDR, COL_SIZE
};
#define CLIST_NVCOLS	(3)		// visible columns
#define CLIST_NCOLS		(3)		// real columns

#define FONT_NAME	"courier"

#define GET_STORE(w)    (GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(w))))

static GtkListStore* clist_create(GtkWidget *list)
{
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkListStore *store;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
    const gchar *text[CLIST_NVCOLS] = { _("Id"), _("Address"), _("Size") };
    gint i;
	
	store = gtk_list_store_new(CLIST_NCOLS,
				G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
				-1
            );
    model = GTK_TREE_MODEL(store);
	
	//clist = tree = gtk_tree_view_new_with_model(model);
	//view = GTK_TREE_VIEW(tree);
  
    gtk_tree_view_set_model(view, model); 
    gtk_tree_view_set_headers_visible(view, TRUE);
	gtk_tree_view_set_rules_hint(view, FALSE);
  
	for(i = COL_ID; i <= COL_SIZE; i++)
	{
		renderer = gtk_cell_renderer_text_new();
		gtk_tree_view_insert_column_with_attributes(view, -1, 
            text[i], renderer, 
            "text", i,
			NULL);
	}
    
    for (i = 0; i < CLIST_NVCOLS; i++) 
    {
		GtkTreeViewColumn *col;
		
		col = gtk_tree_view_get_column(view, i);
		gtk_tree_view_column_set_resizable(col, TRUE);
	}
	
	selection = gtk_tree_view_get_selection(view);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

	return store;
}

static void clist_populate(GtkListStore *store)
{
    int i;
	uint16_t nhandles;

	heap_get_size(&nhandles);
    for(i = 0; i < nhandles; i++)
    {
        GtkTreeIter iter;
        uint32_t addr;
		uint16_t size;
        gchar** row_text = g_malloc0((CLIST_NVCOLS + 1) * sizeof(gchar *));

		heap_get_block_size(i, &addr, &size);

		row_text[0] = g_strdup_printf("%i:", i);
		row_text[1] = g_strdup_printf("$%06x", addr);
		row_text[2] = g_strdup_printf("%i", size);
    
        gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
		COL_ID, row_text[0], COL_ADDR, row_text[1], COL_SIZE, row_text[2],
		-1);
		
		g_strfreev(row_text);
    }
}

static void clist_refresh(GtkListStore *store)
{
	gtk_list_store_clear(store);
	clist_populate(store);
}

static GtkListStore *store = NULL;
static gint already_open = 0;

/*
	Display registers window
*/
GtkWidget* dbgheap_create_window(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
    GtkWidget *data;
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_heap-2.glade"), "dbgheap_window",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgheap_window");

	data = glade_xml_get_widget(xml, "treeview1");
    store = clist_create(data);
	clist_populate(store);

	gtk_tree_view_expand_all(GTK_TREE_VIEW(data));
	gtk_widget_show(data);

	gtk_window_resize(GTK_WINDOW(dbox), options3.heap.w, options3.heap.h);
	gtk_window_move(GTK_WINDOW(dbox), options3.heap.x, options3.heap.y);

    gtk_window_set_transient_for(GTK_WINDOW(dbox), GTK_WINDOW(main_wnd));

	already_open = !0;

	return dbox;
}

GtkWidget* dbgheap_display_window(void)
{
    static GtkWidget *wnd = NULL;

	if(!already_open)
		wnd = dbgheap_create_window();
    gtk_widget_show(wnd);

	clist_refresh(store);

	return wnd;
}

void dbgheap_refresh_window(void)
{
	if(dbgs.heap)
	{
		clist_refresh(store);
	}
}

GLADE_CB gboolean
on_heap_button_press_event        (GtkWidget       *widget,
                                    GdkEventButton  *event,
                                    gpointer         user_data)
{
	GtkWidget *list = GTK_WIDGET(widget);
	GtkTreeView *view = GTK_TREE_VIEW(list);
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GList *l;

	// is double click ?
	if(event->type != GDK_2BUTTON_PRESS)
		return FALSE;
	
	// get selection
	selection = gtk_tree_view_get_selection(view);
	l = gtk_tree_selection_get_selected_rows(selection, &model);
	{
		GtkTreeIter iter;
		GtkTreePath *path = l->data;
        gchar** row_text = g_malloc0((CLIST_NVCOLS + 1) * sizeof(gchar *));
        uint32_t addr;
		
		// get address
		gtk_tree_model_get_iter(model, &iter, path);
		gtk_tree_model_get(model, &iter, COL_ADDR, &row_text[COL_ADDR], -1);

		// show tab
		sscanf(row_text[COL_ADDR], "$%x", &addr);
		dbgmem_add_tab(addr);

        g_strfreev(row_text);
    }

    return FALSE;
}