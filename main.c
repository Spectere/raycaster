#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "game.h"
#include "log.h"
#include "map/map.h"
#include "palette/palette.h"
#include "player.h"
#include "render.h"
#include "video.h"

static char help_text[] = "\n"
		"  -h, --help        displays this help screen\n\n"
		"  -c, --ceiling     defines the ceiling color (in RRGGBB, hex)\n"
		"  -f, --floor       defines the floor color (in RRGGBB, hex)\n"
		"  -p, --palette     loads a palette file\n"
		"  -s, --start       sets the player start position (in X,Y format)\n";

static struct option long_options[] = {
		{ "ceiling",	required_argument,	0, 'c' },
		{ "floor",		required_argument,	0, 'f' },
		{ "help",		no_argument,		0, 'h' },
		{ "palette",	required_argument,	0, 'p' },
		{ "start",		required_argument,	0, 's' },
		{ 0,			0,					0,	0  }
};

void show_usage(char* self) {
	printf("usage: %s [options] map\n", self);
}

void show_help(char* self) {
	show_usage(self);
	printf("%s\n", help_text);
}

int main(int argc, char **argv) {
	int opt, idx;
	double x, y;

	while((opt = getopt_long(argc, argv, "c:f:hp:s:", long_options, &idx)) != -1) {
		switch(opt) {
			case 'c':  /* -c/--ceiling */
				render_set_ceiling_color(optarg);
				break;
			case 'f':  /* -f/--floor */
				render_set_floor_color(optarg);
				break;
			case 'h':  /* -h/--help */
				show_help(argv[0]);
				exit(0);
			case 'p':  /* -p/--palette */
				palette_load(optarg);
				break;
			case 's':  /* -s/--start */
				player_get_coords_from_string(optarg, &x, &y);
				player_set_position_lazy(x, y);
				break;
		}
	}

	if(optind == argc) {
		/* Users didn't specify a map file. */
		show_usage(argv[0]);
		return 1;
	}

	if(!map_load(argv[optind])) {
		lprint(ERROR, "Unable to load map: %s", argv[optind]);
		return 1;
	}

	if(video_init("Raycaster", RENDER_WIDTH, RENDER_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT)) {
		return 1;
	}

	game_loop();
	video_close();

	return 0;
}
