/* player.h */

#ifndef _PLAYER_H
#define _PLAYER_H

#include <SDL.h>

extern double player_x;
extern double player_y;
extern double player_angle;

void player_apply_force(double x, double y);
void player_update();

#endif /* _PLAYER_H */
