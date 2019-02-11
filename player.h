/* player.h */

#ifndef _PLAYER_H
#define _PLAYER_H

#include <SDL.h>

extern double player_x;
extern double player_y;
extern double player_angle;

#define PLAYER_TURN_RIGHT 1
#define PLAYER_TURN_LEFT -1

void player_apply_force(double x, double y);
void player_turn(double delta);
void player_update();

#endif /* _PLAYER_H */
