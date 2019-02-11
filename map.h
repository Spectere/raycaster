/* map.h */

#ifndef _MAP_H
#define _MAP_H

#include <SDL.h>

#define MAP_SIZE_X 32
#define MAP_SIZE_Y 31

#define MAP_POS(x, y) ((MAP_SIZE_X * y) + x)

static const Uint8 map_data[] = {
	0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,
	0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,
	0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0xF,0x0,0xF,0x0,0xF,0x0,0xF,0x0,0xF,0x0,0xF,0x0,0xF,0x0,0x7,
	0x7,0x0,0x0,0x1,0x1,0x0,0x0,0x2,0x2,0x0,0x0,0x3,0x3,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,
	0x7,0x0,0x0,0x1,0x1,0x0,0x0,0x2,0x2,0x0,0x0,0x3,0x3,0x0,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,
	0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,
	0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x8,0x0,0x8,0x0,0x8,0x0,0x8,0x0,0x8,0x0,0x8,0x0,0x8,0x0,0x7,
	0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,
	0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,
	0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x8,0x0,0x8,0x0,0x8,0x0,0x8,0x0,0x8,0x0,0x8,0x0,0x8,0x0,0x7,
	0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,
	0x7,0x0,0x0,0x6,0x6,0x0,0x0,0x5,0x5,0x0,0x0,0x4,0x4,0x0,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x7,
	0x7,0x0,0x0,0x6,0x6,0x0,0x0,0x5,0x5,0x0,0x0,0x4,0x4,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,
	0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0xF,0x0,0xF,0x0,0xF,0x0,0xF,0x0,0xF,0x0,0xF,0x0,0xF,0x0,0x7,
	0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,
	0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x0,0x7,0x0,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,0x7,
	0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x9,0x0,0x9,0x0,0x9,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,
	0x1,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,
	0x1,0x0,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x1,
	0x1,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x1,
	0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x0,0x1,
	0x1,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,
	0x1,0x0,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,
	0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,
	0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x0,0x1,
	0x1,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x1,
	0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x1,0x0,0x1,
	0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x1,
	0x1,0x0,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x1,0x1,0x1,0x0,0x1,0x0,0x1,0x1,0x0,0x1,0x1,0x0,0x1,
	0x1,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,
	0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1
};

#endif /* _MAP_H */
