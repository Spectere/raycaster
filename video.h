/* video.h - Initializes and updates the window. */

#ifndef _VIDEO_H
#define _VIDEO_H

#include <SDL.h>
#include <assert.h>

#include "defs.h"

#define RGB(r, g, b) (0xFF000000 | ((r) << 16) | ((g) << 8) | (b))
#define POS(x, y) ((RENDER_WIDTH * y) + x)

extern Uint32 *pixels;

void video_close();
int video_init(const char *title, int render_width, int render_height,
			   int window_width, int window_height);
void video_update();

#endif /* _VIDEO_H */
