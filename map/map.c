/* map.c */

#include <SDL.h>

#include "../log.h"

#include "map.h"
#include "map_mzm3.h"

int map_size_x = -1;
int map_size_y = -1;
map_tile_t *map_data = NULL;

SDL_bool map_load(char* filename) {
	FILE *f;
	SDL_bool result = SDL_FALSE;

	f = fopen(filename, "rb");
	if(f == NULL) return result;

	/* Look through the map formats until we find the correct one. */
	if(map_mzm3_check(f)) {
		lprint(INFO, "Detected map format: MZM3");
		result = map_mzm3_load(f);
	} else {
		lprint(ERROR, "Unrecognized map format!");
	}

	fclose(f);
	return result;
}
