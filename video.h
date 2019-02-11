/* video.h - Initializes and updates the window. */

#ifndef _VIDEO_H
#define _VIDEO_H

#include <SDL.h>
#include <assert.h>

#include "defs.h"

#define ARGB(a, r, g, b) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#define BLEND(final, col1, col2, mix) final.a = (Uint8)(col1.a * (1 - mix) + col2.a * mix);\
                                      final.r = (Uint8)(col1.r * (1 - mix) + col2.r * mix);\
                                      final.g = (Uint8)(col1.g * (1 - mix) + col2.g * mix);\
                                      final.b = (Uint8)(col1.b * (1 - mix) + col2.b * mix);
#define COL_TO_ARGB(col) ARGB(col.a, col.r, col.g, col.b)
#define POS(x, y) ((RENDER_WIDTH * y) + x)

extern Uint32 *pixels;

void video_close();
int video_init(const char *title, int render_width, int render_height,
			   int window_width, int window_height);
void video_update();

#endif /* _VIDEO_H */
