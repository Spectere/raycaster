/* render.h */

#ifndef _RENDER_H
#define _RENDER_H

#include <SDL.h>

#include "map/map.h"

void render_init();
void render_scene(double view_x, double view_y, double view_angle);
void render_set_ceiling_color(char *hex);
void render_set_floor_color(char *hex);

#endif /* _RENDER_H */
