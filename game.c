/* game.c */

#include "event.h"
#include "game.h"
#include "player.h"
#include "render.h"
#include "texture/texture.h"
#include "video.h"

SDL_bool game_running = SDL_TRUE;

void game_loop() {
	if(!player_init()) exit(1);
	event_init();
	render_init();
	while(game_running) {
		/* TODO: Achieve proper framerate independence. :) */
		event_process();
		player_update();

		video_update_start();
		render_scene(player_x, player_y, player_angle);
		video_update_end();
	}

	/* Cleanup */
	render_quit();
	texture_free();
}
