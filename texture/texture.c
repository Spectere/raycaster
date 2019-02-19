/* texture.c */

#include <SDL.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../log.h"
#include "texture.h"

#include "texture_chr.h"

texture_t *textures;

SDL_bool texture_load(char *filename) {
	FILE *f;
	SDL_bool result = SDL_FALSE;
	struct stat st;

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
		lprint(INFO, "Detected texture format: MZX-style charset (TEXTURE_FORMAT_1BPP)");
		result = texture_chr_load(f);
	} else {
		lprint(WARN, "Unrecognized texture format!");
	}

	fclose(f);
	return result;
}
