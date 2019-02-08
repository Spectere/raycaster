/* game.c */

#include "event.h"
#include "game.h"
#include "player.h"
#include "render.h"
#include "video.h"

SDL_bool game_running = SDL_TRUE;

void game_loop() {
	event_init();
	render_init();
	while(game_running) {
		/* TODO: Achieve proper framerate independence. :) */
		event_process();
		render_scene(player_x, player_y, player_angle);
		video_update();
	}
}
