/* texture.h */

#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <SDL.h>

#include "../palette/palette.h"

typedef enum {
	TEXTURE_FORMAT_1BPP
} texture_format;

typedef struct texture {
	Uint32 width;
	Uint32 height;
	texture_format format;
	color_t (*get_pixel)(int x, int y, color_t fgcolor, color_t bgcolor);
	void *data;
} texture_t;

extern texture_t *textures;

SDL_bool texture_load(char* filename);

#endif /* _TEXTURE_H */
