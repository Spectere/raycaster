#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "game.h"
#include "palette/palette.h"
#include "video.h"

static char help_text[] = "\n"
		"  -h, --help        displays this help screen\n"
		"  -p, --palette     loads a palette file\n";

static struct option long_options[] = {
		{ "help",		no_argument,		0, 'h' },
		{ "palette",	required_argument,	0, 'p' },
		{ 0,			0,					0,	0  }
};

void show_help(char* self) {
	printf("usage: %s [OPTION]\n%s\n", self, help_text);
}

int main(int argc, char **argv) {
	int opt, idx;
	while((opt = getopt_long(argc, argv, "hp:", long_options, &idx)) != -1) {
		switch(opt) {
			case 'h':
				show_help(argv[0]);
				exit(0);
			case 'p':
				palette_load(optarg);
		}
	}

	if(video_init("Raycaster", RENDER_WIDTH, RENDER_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT)) {
		return 1;
	}

	game_loop();
	video_close();

	return 0;
}
