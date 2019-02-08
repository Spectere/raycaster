/* event.c */

#include <math.h>

#include "defs.h"
#include "event.h"
#include "game.h"
#include "map.h"
#include "player.h"

static Uint8 *key_state;

void do_collision() {
    int player_tile_x1 = (int)(player_x - PLAYER_SIZE);
    int player_tile_x2 = (int)(player_x + PLAYER_SIZE);
    int player_tile_y1 = (int)(player_y - PLAYER_SIZE);
    int player_tile_y2 = (int)(player_y + PLAYER_SIZE);

    if(player_tile_x1 < 0 || map_data[MAP_POS(player_tile_x1, (int)player_y)] != 0)
        player_x = player_tile_x1 + 1 + PLAYER_SIZE;
    else if(player_tile_x2 > MAP_SIZE_X - 1 || map_data[MAP_POS(player_tile_x2, (int)player_y)] != 0)
        player_x = player_tile_x2 - PLAYER_SIZE;

    if(player_tile_y1 < 0 || map_data[MAP_POS((int)player_x, player_tile_y1)] != 0)
        player_y = player_tile_y1 + 1 + PLAYER_SIZE;
    else if(player_tile_y2 > MAP_SIZE_Y - 1 || map_data[MAP_POS((int)player_x, player_tile_y2)] != 0)
        player_y = player_tile_y2 - PLAYER_SIZE;
}

void event_init() {
	key_state = SDL_GetKeyboardState(NULL);
}

void event_keyboard(const SDL_Event e) {
	SDL_KeyboardEvent kb = e.key;

	if(kb.type == SDL_KEYUP) {
		if(kb.keysym.sym == SDLK_ESCAPE)
			game_running = SDL_FALSE;
	}
}

void event_process() {
	SDL_Event e;
	double move_speed = MOVE_SPEED;
	double turn_speed = TURN_SPEED;
	double player_angle_rads = DEG2RAD(player_angle);

	/* Handle movement keys first. */
	if(key_state[SDL_SCANCODE_LSHIFT] || key_state[SDL_SCANCODE_RSHIFT]) {
		move_speed *= 2;
		turn_speed *= 2;
	}

	if(key_state[SDL_SCANCODE_UP] || key_state[SDL_SCANCODE_W]) {
		player_x += sin(player_angle_rads) * move_speed;
		player_y -= cos(player_angle_rads) * move_speed;
	}

	if(key_state[SDL_SCANCODE_DOWN] || key_state[SDL_SCANCODE_S]) {
		player_x -= sin(player_angle_rads) * move_speed;
		player_y += cos(player_angle_rads) * move_speed;
	}

	if(key_state[SDL_SCANCODE_LEFT]) {
		player_angle -= turn_speed;
	}

	if(key_state[SDL_SCANCODE_RIGHT]) {
		player_angle += turn_speed;
	}

	if(key_state[SDL_SCANCODE_A]) {
		player_x += sin(player_angle_rads - (M_PI / 2)) * move_speed;
		player_y -= cos(player_angle_rads - (M_PI / 2)) * move_speed;
	}

	if(key_state[SDL_SCANCODE_D]) {
		player_x += sin(player_angle_rads + (M_PI / 2)) * move_speed;
		player_y -= cos(player_angle_rads + (M_PI / 2)) * move_speed;
	}

	do_collision();

	if(player_x > MAP_SIZE_X - 0.01) player_x = MAP_SIZE_X - 0.01;
	if(player_y > MAP_SIZE_Y - 0.01) player_y = MAP_SIZE_Y - 0.01;
	if(player_x < 0) player_x = 0;
	if(player_y < 0) player_y = 0;

	while(SDL_PollEvent(&e)) {
		switch(e.type) {
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				event_keyboard(e);
				break;

			case SDL_QUIT:
				game_running = SDL_FALSE;
		}
	}
}
