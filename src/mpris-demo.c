#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <glib-object.h>

#include <glade/glade.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>

GtkWidget	  *window,
		  *cbox_players;
GladeXML	  *xml;

#if 0
typedef struct _FOOBAR FOOBAR;
struct _FOOBAR {
  GList		  *players;
  MPRISConnection *connection;
};

void
on_button_connect_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  MPRISPlayerInfo *p_info = NULL; 
  gint id;

  id = 
      gtk_combo_box_get_active (GTK_COMBO_BOX(glade_xml_get_widget(xml, "cbox_players")));
  p_info = 
      (MPRISPlayerInfo*)g_list_nth_data (foobar->players, id); 

  foobar->connection = mpris_connection_establish (p_info); 

  if (foobar->connection)
      gtk_image_set_from_icon_name (GTK_IMAGE(glade_xml_get_widget(xml, "image_connection")), "gtk-yes", GTK_ICON_SIZE_MENU);
  else
      gtk_image_set_from_icon_name (GTK_IMAGE(glade_xml_get_widget(xml, "image_connection")), "gtk-no", GTK_ICON_SIZE_MENU);
}

void
on_button_pause_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  mpris_connection_invoke_method (foobar->connection, MPRIS_METHOD_PLAY_PAUSE);
}

void
on_button_prev_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  mpris_connection_invoke_method (foobar->connection, MPRIS_METHOD_PLAY_PREV);
}

void
on_button_next_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  mpris_connection_invoke_method (foobar->connection, MPRIS_METHOD_PLAY_NEXT);
}

void
on_button_stop_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  mpris_connection_invoke_method (foobar->connection, MPRIS_METHOD_PLAY_STOP);
}

void
on_button_play_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  mpris_connection_invoke_method (foobar->connection, MPRIS_METHOD_PLAY_CURRENT);
}
#endif

gint
main (gint n_args, gchar **args)
{
#if 0
  FOOBAR	  *foobar = g_new0 (FOOBAR,1);
#endif
  GList		  *iter_list = NULL;
  const gchar	  *xml_file  = DATA_DIR "/glade/mpris-demo.glade";

  gtk_init (&n_args, &args);

  xml = glade_xml_new (xml_file, NULL, NULL);
  window = glade_xml_get_widget (xml, "window");

//  foobar->players = mpris_list ();
  mpris_list ();

#if 0
  if (!foobar->players)
    g_message ("Couldn't get list of MPRIS compliant services"); 
#endif

#if 0
  cbox_players = glade_xml_get_widget (xml, "cbox_players");
  gtk_combo_box_remove_text (GTK_COMBO_BOX(cbox_players), 0);

  iter_list = foobar->players;
  for ( ; iter_list ; iter_list = iter_list->next ) 
    {
	MPRISPlayerInfo *p_info = (MPRISPlayerInfo*)(iter_list->data);
	gtk_combo_box_append_text (GTK_COMBO_BOX(cbox_players), p_info->name); 
    }

  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_connect")), "clicked", G_CALLBACK(on_button_connect_clicked), foobar);

  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_pause")), "clicked", G_CALLBACK(on_button_pause_clicked), foobar);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_prev")), "clicked",  G_CALLBACK(on_button_prev_clicked), foobar);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_next")), "clicked",  G_CALLBACK(on_button_next_clicked), foobar);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_stop")), "clicked",  G_CALLBACK(on_button_stop_clicked), foobar);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_play")), "clicked",  G_CALLBACK(on_button_play_clicked), foobar);

  gtk_widget_show_all (window);

  gtk_main ();
#endif

  exit (0);
}
