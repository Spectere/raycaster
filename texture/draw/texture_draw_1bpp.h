/* texture_draw_1bpp.h */

#ifndef _TEXTURE_DRAW_1BPP_H
#define _TEXTURE_DRAW_1BPP_H

#include "../../palette/palette.h"
#include "../texture.h"

color_t texture_draw_1bpp_get_pixel(texture_t *tex, int x, int y, color_t fgcolor, color_t bgcolor);

#endif /* _TEXTURE_DRAW_1BPP_H */
