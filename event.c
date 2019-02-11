/* event.c */

#include <math.h>

#include "defs.h"
#include "event.h"
#include "game.h"
#include "map.h"
#include "player.h"

static const Uint8 *key_state;

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
	double move_multiplier = 1.0;
	double turn_speed = TURN_SPEED;
	double player_angle_rads = DEG2RAD(player_angle);
	double player_move_x = 0.0, player_move_y = 0.0;

	/* Handle movement keys first. */
	if(key_state[SDL_SCANCODE_LSHIFT] || key_state[SDL_SCANCODE_RSHIFT]) {
		move_multiplier = 2.0;
		turn_speed *= 2;
	}

	if(key_state[SDL_SCANCODE_UP] || key_state[SDL_SCANCODE_W]) {
		player_move_x += sin(player_angle_rads) * move_multiplier;
		player_move_y -= cos(player_angle_rads) * move_multiplier;
	}

	if(key_state[SDL_SCANCODE_DOWN] || key_state[SDL_SCANCODE_S]) {
		player_move_x -= sin(player_angle_rads) * move_multiplier;
		player_move_y += cos(player_angle_rads) * move_multiplier;
	}

	if(key_state[SDL_SCANCODE_LEFT]) {
		player_angle -= turn_speed;
	}

	if(key_state[SDL_SCANCODE_RIGHT]) {
		player_angle += turn_speed;
	}

	if(key_state[SDL_SCANCODE_A]) {
		player_move_x += sin(player_angle_rads - (M_PI / 2)) * move_multiplier;
		player_move_y -= cos(player_angle_rads - (M_PI / 2)) * move_multiplier;
	}

	if(key_state[SDL_SCANCODE_D]) {
		player_move_x += sin(player_angle_rads + (M_PI / 2)) * move_multiplier;
		player_move_y -= cos(player_angle_rads + (M_PI / 2)) * move_multiplier;
	}

	player_apply_force(player_move_x, player_move_y);

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
