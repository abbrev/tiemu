/* Hey EMACS -*- linux-c -*- */
/* $Id: dbg_code.c 731 2004-11-28 16:05:10Z roms $ */

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

#include "intl.h"
#include "support.h"
#include "ti68k_int.h"
#include "struct.h"
#include "romcalls.h"

enum {
		COL_ID, COL_NAME, COL_ADDR,
};
#define CLIST_NVCOLS	(3)		// 3 visible columns
#define CLIST_NCOLS		(3)		// 3 real columns

#define FONT_NAME	"courier"


GLADE_CB gboolean    
on_combo_entry1_match_selected             (GtkEntryCompletion *widget,
                                            GtkTreeModel *model,
                                            GtkTreeIter *iter,
                                            gpointer user_data);

void dbgromcall_fill_window(GtkWidget *widget)
{
	GtkListStore *store;
	GtkTreeModel *model;
	GList *lst, *ptr;

	GtkEntry *entry = GTK_ENTRY(GTK_BIN(GTK_COMBO_BOX(widget))->child);
	GtkEntryCompletion* completion;

	//gtk_combo_box_set_add_tearoffs(GTK_COMBO_BOX(widget), FALSE);

	// create storage
	store = gtk_list_store_new(CLIST_NCOLS,
				G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
				-1
            );
    model = GTK_TREE_MODEL(store);
	gtk_combo_box_set_model(GTK_COMBO_BOX(widget), model);

	// fill storage
	lst = romcalls_sort_by_iname();
	if(lst == NULL)
		return;

	for(ptr = lst; ptr != NULL; ptr = g_list_next(ptr))
	{
		uint32_t addr = ROMCALL_ADDR(ptr);
		const gchar *name = ROMCALL_NAME(ptr);
		int id = ROMCALL_ID(ptr);
		gchar** row_text = g_malloc0((CLIST_NVCOLS + 1) * sizeof(gchar *));
		GtkTreeIter iter;

		if(!strcmp(name, "unknown") || (name == NULL))
			continue;

		row_text[0] = g_strdup_printf("#%03x", id);
		row_text[1] = g_strdup(name);
		row_text[2] = g_strdup_printf("[$%x]", addr);

		gtk_list_store_append(store, &iter);
	    gtk_list_store_set(store, &iter, 
	    COL_ID, row_text[0], 
		COL_NAME, row_text[1],
        COL_ADDR, row_text[2],
		-1);

		g_strfreev(row_text);
	}

	gtk_combo_box_entry_set_text_column(GTK_COMBO_BOX_ENTRY(widget), COL_NAME);

	/* --- */

	// set auto-completion
	completion = gtk_entry_completion_new();
	gtk_entry_set_completion(entry, completion);
	gtk_entry_completion_set_model(completion, model);
	gtk_entry_completion_set_text_column (completion, COL_NAME);
	g_signal_connect(G_OBJECT(completion), "match-selected", 
		G_CALLBACK(on_combo_entry1_match_selected), NULL);

	//gtk_editable_select_region(GTK_EDITABLE(entry), 0, -1);
}

static void goto_romcall(const char *str)
{
	/*
	uint32_t addr;
	int id;
	gchar name[256];
	int ret;

	ret = sscanf(str, "%s [$%x] - #%03x ",name, &addr, &id);
	if(ret == 3)
		dbgcode_disasm_at(addr & 0xffffff);
	*/
	printf("changed: <%s>\n", str);
}

GLADE_CB void
on_combo_entry1_changed                    (GtkComboBox *combobox,
                                            gpointer user_data)
{
	GtkEntry *entry = GTK_ENTRY(GTK_BIN(combobox)->child);

	gchar *str = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
	goto_romcall(str);
	g_free(str);
}

GLADE_CB gboolean    
on_combo_entry1_match_selected             (GtkEntryCompletion *completion,
                                            GtkTreeModel *model,
                                            GtkTreeIter *iter,
                                            gpointer user_data)
{
	GtkWidget *entry = gtk_entry_completion_get_entry(completion);

	gchar *str = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
	goto_romcall(str);
	g_free(str);
	return FALSE;
}