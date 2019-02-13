/* map_mzm3.c */

#include <SDL.h>
#include <stdio.h>

#include "../log.h"

#include "map_mzm3.h"

static const char mzm3_magic[] = { 'M', 'Z', 'M', '3' };

SDL_bool map_mzm3_check(FILE *f) {
	char buf[4];
	int i;

	fseek(f, 0, SEEK_SET);
	fread(buf, 4, 1, f);

	for(i = 0; i < 4; i++) {
		if(buf[i] != mzm3_magic[i])
			return SDL_FALSE;
	}

	return SDL_TRUE;
}

SDL_bool map_mzm3_load(FILE *f) {
	mzm3_header_t header;

	fseek(f, 0, SEEK_SET);
	fread(&header, sizeof(mzm3_header_t), 1, f);

	switch(header.storage_mode) {
		case MZM3_BOARD:
			return map_mzm3_board_load(f, header);
		case MZM3_LAYER:
			lprint(ERROR, "MZM3 layer mode is currently not supported.");
			return SDL_FALSE;
		default:
			lprint(ERROR, "Unrecognized MZM3 storage mode.");
			return SDL_FALSE;
	}
}
