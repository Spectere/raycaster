/* video.c - Initializes and updates the window. */

#include <stdlib.h>

#include "log.h"
#include "video.h"

int tex_width, tex_height, wnd_width, wnd_height;

Uint32 *pixels;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

void video_close() {
	lprint(INFO, "Closing video subsystem");

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

int video_init(const char *title, int render_width, int render_height,
			   int window_width, int window_height) {
	tex_width = render_width;
	tex_height = render_height;
	wnd_width = window_width;
	wnd_height = window_height;

	SDL_Init(SDL_INIT_VIDEO);

	lprint(INFO, "Initializing video (%ix%i surface, %ix%i window)...",
		   tex_width, tex_height, wnd_width, wnd_height);

	/* Create an SDL window. */
	lprint(DEBUG, "Creating window");
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED, window_width, window_height, 0);
	if(window == NULL) {
		lprint(ERROR, "Error creating window: %s", SDL_GetError());
		return 1;
	}

	/* Create the renderer. */
	lprint(DEBUG, "Creating hardware accelerated renderer");
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL) {
		/* Attempt to fall back to software rendering. */
		lprint(WARN, "Failed to create hardware accelerated renderer: %s", SDL_GetError());
		lprint(WARN, "Attempting to create a software renderer");
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
		if(renderer == NULL) {
			lprint(ERROR, "Error creating renderer: %s", SDL_GetError());
			return 1;
		}
	}

	/* Create an ARGB8888 texture. */
	lprint(DEBUG, "Creating texture");
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
								SDL_TEXTUREACCESS_STREAMING, tex_width, tex_height);
	if(texture == NULL) {
		lprint(ERROR, "Error creating texture: %s", SDL_GetError());
		return 1;
	}

	pixels = malloc(sizeof(Uint32) * tex_width * tex_height);

	lprint(INFO, "Video subsystem initialized successfully");
	return 0;
}

void video_update() {
	SDL_UpdateTexture(texture, NULL, pixels, tex_width * sizeof(Uint32));
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
