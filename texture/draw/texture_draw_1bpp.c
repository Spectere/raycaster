/* texture_draw_1bpp.c */

#include "../../palette/palette.h"
#include "../texture.h"
#include "texture_draw_1bpp.h"

color_t texture_draw_1bpp_get_pixel(texture_t *tex, int x, int y, color_t fgcolor, color_t bgcolor) {
    return ((Uint8*)tex->data)[y] >> ((tex->width - 1) - x) & 0x01
        ? fgcolor
        : bgcolor;
}
