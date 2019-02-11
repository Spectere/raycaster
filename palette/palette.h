/* palette.h */

#ifndef _PALETTE_H
#define _PALETTE_H

#include <SDL.h>

#include "../defs.h"
#include "../video.h"

typedef struct color {
	Uint8 a;
	Uint8 r;
	Uint8 g;
	Uint8 b;
} color_t;

extern const color_t palette_cga[];

#define SHADE_ARGB(col) ARGB(col.a,\
                            (Uint8)(col.r / SHADE_FACTOR),\
                            (Uint8)(col.g / SHADE_FACTOR),\
                            (Uint8)(col.b / SHADE_FACTOR))

#define SHADE_COL(src, dest) dest.a = src.a;\
                             dest.r = (Uint8)(src.r / SHADE_FACTOR);\
                             dest.g = (Uint8)(src.g / SHADE_FACTOR);\
                             dest.b = (Uint8)(src.b / SHADE_FACTOR);


#endif /* _PALETTE_H */
