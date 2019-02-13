/* map.h */

#ifndef _MAP_H
#define _MAP_H

#include <SDL.h>

#define MAP_POS(x, y) ((map_size_x * y) + x)
#define GET_COORD(i, x, y) { y = i / map_size_x; x = i % map_size_y; }

typedef struct map_tile {
	Uint8 texture;
	Uint8 fg_color;
	Uint8 bg_color;
} map_tile_t;

extern int map_size_x;
extern int map_size_y;
extern map_tile_t *map_data;

SDL_bool map_load(char* filename);

#endif /* _MAP_H */
