/* game.c */

#include "event.h"
#include "game.h"
#include "player.h"
#include "render.h"
#include "video.h"

SDL_bool game_running = SDL_TRUE;

void game_loop() {
	Uint8 r = 0, g = 64, b = 128;
	int i;
	event_init();
	while(game_running) {
		/* TODO: Achieve proper framerate independence. :) */
		event_process();
		render_scene(player_x, player_y, player_angle);
		video_update();
	}
}
