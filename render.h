/* render.h */

#ifndef _RENDER_H
#define _RENDER_H

#include <SDL.h>

#include "map.h"

void render_init();
void render_scene(double view_x, double view_y, double view_angle);

#endif /* _RENDER_H */
