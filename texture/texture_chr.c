/* texture_chr.c */

#include <SDL.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../log.h"
#include "draw/texture_draw_1bpp.h"
#include "texture_chr.h"
#include "texture.h"

SDL_bool texture_chr_check(FILE *f, struct stat *st) {
	return st->st_size % 256 == 0;
}

SDL_bool texture_chr_load(FILE *f, struct stat *st) {
	const int WIDTH = 8, COUNT= 256;
	Uint32 height, i;

	/* This format is headerless and only supports 256 bitmaps, at 8 pixels
	 * wide. The only option to get the height is to divide the file size by
	 * 256. Partial charsets are not supported. */
	height = (Uint32)(st->st_size / COUNT);
	lprint(DEBUG, "1BPP charset contains %i glyphs at %ix%i", COUNT, WIDTH, height);

	if(!texture_init(COUNT)) {
		lprint(WARN, "Unable to allocate texture memory");
		return SDL_FALSE;
	}

	for(i = 0; i < COUNT; i++) {
		textures[i].width = WIDTH;
		textures[i].height = height;
		textures[i].format = TEXTURE_FORMAT_1BPP;
		textures[i].loaded = SDL_TRUE;
		textures[i].get_pixel = texture_draw_1bpp_get_pixel;
		textures[i].data = malloc(sizeof(Uint8) * height);

		fread(textures[i].data, sizeof(Uint8) * height, 1, f);
	}

	return SDL_TRUE;
}
