#include <mpris/mpris.h>

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

struct option long_options[] = {
	{"prev",		0,	NULL,   'p'},
	{"next",		0,	NULL,   'n'},
	{"pause", 		0,	NULL,   'a'},
	{"play",		0,	NULL,   'P'},
	{"stop",		0,	NULL,   's'},
	{"metadata", 	0, 	NULL, 	'm'},
	{"loopon",		0,  NULL,   'l'},
	{"loopoff",	 	0,  NULL,   'L'},
	{"randomon",	0,  NULL,   'r'},
	{"randomoff", 	0,  NULL,   'R'},
	{"deltrack", 	1,  NULL,   'd'},
	{"getlength", 	0,  NULL,   'g'},
	{0,0,0,0}
};

int main(int argc, char** argv)
{
	MPRISPlayer *player = NULL;
	MPRISPlayerInfo **p_list = NULL;
	MPRISMetadata *metadata = NULL;
	int opt;
	int init;

	init = mpris_client_init();

	p_list = mpris_list_players();
	
	if (p_list == NULL)
		return 1;

	/* Get the first available player */
	player = mpris_player_new(p_list[0]->suffix);

	while ((opt = getopt_long(argc, argv,
					"c:dfkmnl::v", long_options, NULL)) != -1) {
		switch (opt) {
			case 'p':
				mpris_player_prev(player);
				break;
			case 'n':
				mpris_player_next(player);
				break;
			case 'a':
				mpris_player_pause(player);
				break;
			case 'P':
				mpris_player_play(player);
				break;
			case 's':
				mpris_player_stop(player);
				break;
			case 'l':
				mpris_tracklist_set_loop(player, 1);
				break;
			case 'L':
				mpris_tracklist_set_loop(player, 0);
				break;
			case 'r':
				mpris_tracklist_set_random(player, 1);
				break;
			case 'R':
				mpris_tracklist_set_random(player, 0);
				break;
			case 'd':
				mpris_tracklist_del_track(player, atoi(optarg));
				break;
			case 'g':
				printf("Elements: %d\n", mpris_tracklist_get_length(player));
				break;
			case 'm':
				metadata = mpris_metadata_get_current_track(player);
				printf ("Changed Track !\n"
						"New metadata is:\n"
						"location: %s\n"
						"Title: %s\n"
						"Artist: %s\n"
						"Album: %s\n"
						"Tracknumber: %s\n"
						"Time: %d\n"
						"mTime: %d\n"
						"Genere: %s\n"
						"Comment: %s\n"
						"Rating: %d\n"
						"Year: %d\n"
						"Date: %d\n"
						"At URL: %s\n",
						metadata->location,
						metadata->title,
						metadata->artist,
						metadata->album,
						metadata->tracknumber,
						metadata->time,
						metadata->mtime,
						metadata->genere,
						metadata->comment,
						metadata->rating,
						metadata->year,
						metadata->date,
						metadata->aturl
							);
				break;
		}
	}

	mpris_metadata_free(metadata);

	mpris_player_free(player);

	return 0;
}

