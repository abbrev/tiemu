/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "state_cb.h"
#include "state_dbox.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_fs_load_state (void)
{
  GtkWidget *fs_load_state;
  GtkWidget *ok_button1;
  GtkWidget *cancel_button1;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  fs_load_state = gtk_file_selection_new (_("Select File"));
  gtk_container_set_border_width (GTK_CONTAINER (fs_load_state), 10);

  ok_button1 = GTK_FILE_SELECTION (fs_load_state)->ok_button;
  gtk_widget_show (ok_button1);
  GTK_WIDGET_SET_FLAGS (ok_button1, GTK_CAN_DEFAULT);
  gtk_widget_add_accelerator (ok_button1, "clicked", accel_group,
                              GDK_Return, 0,
                              GTK_ACCEL_VISIBLE);

  cancel_button1 = GTK_FILE_SELECTION (fs_load_state)->cancel_button;
  gtk_widget_show (cancel_button1);
  GTK_WIDGET_SET_FLAGS (cancel_button1, GTK_CAN_DEFAULT);
  gtk_widget_add_accelerator (cancel_button1, "clicked", accel_group,
                              GDK_Escape, 0,
                              GTK_ACCEL_VISIBLE);

  g_signal_connect ((gpointer) fs_load_state, "destroy",
                    G_CALLBACK (on_fs_load_state_destroy),
                    NULL);
  g_signal_connect ((gpointer) ok_button1, "clicked",
                    G_CALLBACK (load_state_on_ok_button1_clicked),
                    NULL);
  g_signal_connect ((gpointer) cancel_button1, "clicked",
                    G_CALLBACK (load_state_on_cancel_button1_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (fs_load_state, fs_load_state, "fs_load_state");
  GLADE_HOOKUP_OBJECT_NO_REF (fs_load_state, ok_button1, "ok_button1");
  GLADE_HOOKUP_OBJECT_NO_REF (fs_load_state, cancel_button1, "cancel_button1");

  gtk_window_add_accel_group (GTK_WINDOW (fs_load_state), accel_group);

  return fs_load_state;
}

GtkWidget*
create_fs_save_state (void)
{
  GtkWidget *fs_save_state;
  GtkWidget *ok_button2;
  GtkWidget *cancel_button2;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  fs_save_state = gtk_file_selection_new (_("Select File"));
  gtk_container_set_border_width (GTK_CONTAINER (fs_save_state), 10);

  ok_button2 = GTK_FILE_SELECTION (fs_save_state)->ok_button;
  gtk_widget_show (ok_button2);
  GTK_WIDGET_SET_FLAGS (ok_button2, GTK_CAN_DEFAULT);
  gtk_widget_add_accelerator (ok_button2, "clicked", accel_group,
                              GDK_Return, 0,
                              GTK_ACCEL_VISIBLE);

  cancel_button2 = GTK_FILE_SELECTION (fs_save_state)->cancel_button;
  gtk_widget_show (cancel_button2);
  GTK_WIDGET_SET_FLAGS (cancel_button2, GTK_CAN_DEFAULT);
  gtk_widget_add_accelerator (cancel_button2, "clicked", accel_group,
                              GDK_Escape, 0,
                              GTK_ACCEL_VISIBLE);

  g_signal_connect ((gpointer) fs_save_state, "destroy",
                    G_CALLBACK (on_fs_save_state_destroy),
                    NULL);
  g_signal_connect ((gpointer) ok_button2, "clicked",
                    G_CALLBACK (save_state_on_ok_button2_clicked),
                    NULL);
  g_signal_connect ((gpointer) cancel_button2, "clicked",
                    G_CALLBACK (save_state_on_cancel_button2_pressed),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (fs_save_state, fs_save_state, "fs_save_state");
  GLADE_HOOKUP_OBJECT_NO_REF (fs_save_state, ok_button2, "ok_button2");
  GLADE_HOOKUP_OBJECT_NO_REF (fs_save_state, cancel_button2, "cancel_button2");

  gtk_window_add_accel_group (GTK_WINDOW (fs_save_state), accel_group);

  return fs_save_state;
}

