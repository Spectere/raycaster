/* texture.c */

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../log.h"
#include "texture.h"

#include "texture_chr.h"

static int texture_count = 0;
texture_t *textures = NULL;

void texture_free() {
	int i;

	if(textures == NULL) return;

	lprint(DEBUG, "Freeing loaded texture set");

	for(i = 0; i < texture_count; i++) {
		/* If the texture is loaded, its data should be freed. */
		if(textures[i].loaded)
			free(textures[i].data);
	}

	free(textures);

	texture_count = 0;
	textures = NULL;
}

SDL_bool texture_init(size_t count) {
	int i;

	lprint(DEBUG, "Initializing %i textures", count);

	textures = malloc(sizeof(texture_t) * count);
	if(textures == NULL) {
		return SDL_FALSE;
	}

	/* Create empty texture. */
	textures[0].width = textures[0].height = 0;
	textures[0].format = TEXTURE_FORMAT_NONE;
	textures[0].get_pixel = NULL;
	textures[0].loaded = SDL_FALSE;
	textures[0].data = NULL;

	/* Copy that to every texture created. */
	for(i = 1; i < count; i++)
		memcpy(&textures[i], &textures[0], sizeof(texture_t));

	return SDL_TRUE;
}

SDL_bool texture_load(char *filename) {
	FILE *f;
	SDL_bool result = SDL_FALSE;
	struct stat st;

	if(textures != NULL) texture_free();

	lprint(INFO, "Loading texture set: %s", filename);

	if(lstat(filename, &st) == -1) {
		lprint(WARN, "Could not stat file: %s", filename);
		return result;
	}

	f = fopen(filename, "rb");
	if(f == NULL) {
		lprint(WARN, "Unable to open texture set: %s", filename);
		return result;
	}

	if(texture_chr_check(f, &st)) {
		lprint(DEBUG, "Detected texture format: MZX-style charset (TEXTURE_FORMAT_1BPP)");
		result = texture_chr_load(f, &st);
	} else {
		lprint(WARN, "Unrecognized texture format!");
	}

	fclose(f);
	return result;
}
