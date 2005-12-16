#include <stdlib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>
#include <mpris/mpris-list.h>


void
demo_set_stream_pos (MPRISPlayer *player, int32_t stream_pos)
{
    GtkWidget *w_position = glade_xml_get_widget (GLADE_XML(player->user_data), "position");
    gchar *aux;
    gint minutes, seconds;

    minutes = stream_pos / 60;
    seconds = stream_pos % 60;

    aux = g_strdup_printf("%d:%-2.2d",minutes,seconds);
    gtk_label_set_text (w_position, aux);
    g_free (aux);

}

void
demo_track_change (MPRISPlayer *player)
{
    GtkWidget *w_title = glade_xml_get_widget (GLADE_XML(player->user_data), "title");
    gchar *aux;

    mpris_player_invoke_method (player, MPRIS_METHOD_GET_CURRENT_TITLE, &aux);
    gtk_label_set_text (w_title, aux);
}

void
demo_set_volume (MPRISPlayer *player, int32_t volume)
{
    GtkWidget *w_volume = glade_xml_get_widget (GLADE_XML(player->user_data), "volume");
    gchar *aux;

    aux = g_strdup_printf ("%d", volume);
    gtk_label_set_text (w_volume, aux);
    g_free (aux);
}

typedef struct {
  MPRISPlayer *player;
  gint	       method;
} MethodCall;
 
gboolean
mpris_button (GtkWidget *widget, MethodCall *call)
{
    MethodCall _call = *call;
    mpris_player_invoke_method (_call.player, _call.method);
    return FALSE;
}


int
main (int n_args, char **args)
{
  MPRISPlayer *player;

  GladeXML    *xml;
  GtkWidget   *window;

  int32_t volume;
  char   *title;

  MethodCall play,pause,prev,next,stop;

  gtk_init (&n_args, &args);
  xml = glade_xml_new (DATA_DIR "/glade/mpris-demo.glade", NULL, NULL);
  window = glade_xml_get_widget (xml, "window1");
  gtk_widget_show_all (window);

  mpris_client_init ();

  player = mpris_player_new ("bmpx",
			     demo_set_stream_pos,
			     NULL,
			     NULL,
			     demo_set_volume);

  player->user_data = xml;

  /* Hook up buttons */
  prev.player = player;
  prev.method = MPRIS_METHOD_PLAY_PREV;

  next.player = player;
  next.method = MPRIS_METHOD_PLAY_NEXT;

  play.player = player;
  play.method = MPRIS_METHOD_PLAY_CURRENT;

  pause.player = player;
  pause.method = MPRIS_METHOD_PLAY_PAUSE;

  stop.player = player;
  stop.method = MPRIS_METHOD_PLAY_STOP;

  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_pause")), "clicked", mpris_button, &pause); 
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_next")), "clicked", mpris_button, &next);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_prev")), "clicked", mpris_button, &prev);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_stop")), "clicked", mpris_button, &stop);
  g_signal_connect (G_OBJECT(glade_xml_get_widget(xml, "button_play")), "clicked", mpris_button, &play);

  mpris_player_invoke_method (player, MPRIS_METHOD_GET_VOLUME, &volume);  
  demo_set_volume (player, volume);
  demo_track_change (player);

  mpris_player_start_listen (player);

  g_timeout_add (250, gtk_widget_queue_draw, window);

  gtk_main ();

  mpris_player_stop_listen (player);

  exit (0);
}
