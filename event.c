/* event.c */

#include "event.h"
#include "game.h"

void event_keyboard(const SDL_Event e) {
	SDL_KeyboardEvent kb = e.key;

	if(kb.type == SDL_KEYUP) {
		switch(kb.keysym.sym) {
			case SDLK_ESCAPE:
				game_running = SDL_FALSE;
		}
	}
}

void event_process() {
	SDL_Event e;

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
