#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <glib-object.h>

#include <glade/glade.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>
#include <mpris/mpris-list.h>

GtkWidget	  *window, *cbox_players;
GladeXML	  *xml;

typedef struct _FOOBAR FOOBAR;
struct _FOOBAR {
  MPRISPlayerInfo  **players;
  MPRISPlayerInfo   *current;
};

void
on_button_connect_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  gint id;

  id = 
      gtk_combo_box_get_active (GTK_COMBO_BOX(glade_xml_get_widget(xml, "cbox_players")));
  foobar->current = 
      (MPRISPlayerInfo*)foobar->players[id];
}

void
on_button_pause_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  mpris_invoke_method (foobar->current, MPRIS_METHOD_PLAY_PAUSE);
}

void
on_button_prev_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  mpris_invoke_method (foobar->current, MPRIS_METHOD_PLAY_PREV);
}

void
on_button_next_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  mpris_invoke_method (foobar->current, MPRIS_METHOD_PLAY_NEXT);
}

void
on_button_stop_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  mpris_invoke_method (foobar->current, MPRIS_METHOD_PLAY_STOP);
}

void
on_button_play_clicked (GtkWidget *widget, FOOBAR *foobar)
{
  mpris_invoke_method (foobar->current, MPRIS_METHOD_PLAY_CURRENT);
}

gint
main (gint n_args, gchar **args)
{
  FOOBAR	   *foobar = g_new0 (FOOBAR,1);
  const gchar	   *xml_file  = DATA_DIR "/glade/mpris-demo.glade";
  gint		    n = 0;

  gtk_init (&n_args, &args);
  mpris_init ();

  xml = glade_xml_new (xml_file, NULL, NULL);
  window = glade_xml_get_widget (xml, "window");
  cbox_players = glade_xml_get_widget (xml, "cbox_players");
  gtk_combo_box_remove_text (GTK_COMBO_BOX(cbox_players), 0);

  foobar->players = mpris_list_clients ();

  if (!foobar->players)
    {
      fprintf (stderr, "%s(%d): Couldn't get list of MPRIS compliant services", __FILE__, __LINE__); 
      exit (1);
    }

  while (foobar->players[n])
    {
      MPRISPlayerInfo *p_info = (MPRISPlayerInfo*)foobar->players[n];
      printf("entry = %s\n", p_info->path); 
      gtk_combo_box_append_text (GTK_COMBO_BOX(cbox_players), p_info->name); 
      n++;
    }

  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_connect")), "clicked", G_CALLBACK(on_button_connect_clicked), foobar);

  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_pause")), "clicked", G_CALLBACK(on_button_pause_clicked), foobar);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_prev")), "clicked",  G_CALLBACK(on_button_prev_clicked), foobar);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_next")), "clicked",  G_CALLBACK(on_button_next_clicked), foobar);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_stop")), "clicked",  G_CALLBACK(on_button_stop_clicked), foobar);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_play")), "clicked",  G_CALLBACK(on_button_play_clicked), foobar);

  gtk_widget_show_all (window);

  gtk_main ();

  exit (0);
}
