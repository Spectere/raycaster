/* player.c */

#include <math.h>

#include "defs.h"
#include "map.h"
#include "player.h"

double player_x = PLAYER_START_X;
double player_y = PLAYER_START_Y;
double player_angle = PLAYER_START_ANGLE;

static double player_speed_x = 0.0;
static double player_speed_y = 0.0;
static double player_spin = 0.0;

void player_apply_force(double x, double y) {
    if(fabs(player_speed_x) + fabs(player_speed_y) >= MOVE_SPEED_MAX)
        return;

    player_speed_x += x * MOVE_SPEED;
    player_speed_y += y * MOVE_SPEED;
}

void player_collision() {
    int player_tile_x1 = (int)(player_x - PLAYER_SIZE);
    int player_tile_x2 = (int)(player_x + PLAYER_SIZE);
    int player_tile_y1 = (int)(player_y - PLAYER_SIZE);
    int player_tile_y2 = (int)(player_y + PLAYER_SIZE);

    if(player_x < PLAYER_SIZE)
        player_x = PLAYER_SIZE;
    else if(map_data[MAP_POS(player_tile_x1, (int)player_y)] != 0)
        player_x = player_tile_x1 + 1 + PLAYER_SIZE;
    else if(player_tile_x2 > MAP_SIZE_X - 1 || map_data[MAP_POS(player_tile_x2, (int)player_y)] != 0)
        player_x = player_tile_x2 - PLAYER_SIZE;

    if(player_y < PLAYER_SIZE)
        player_y = PLAYER_SIZE;
    else if(map_data[MAP_POS((int)player_x, player_tile_y1)] != 0)
        player_y = player_tile_y1 + 1 + PLAYER_SIZE;
    else if(player_tile_y2 > MAP_SIZE_Y - 1 || map_data[MAP_POS((int)player_x, player_tile_y2)] != 0)
        player_y = player_tile_y2 - PLAYER_SIZE;
}

void player_turn(double delta) {
    if(fabs(player_spin) >= TURN_SPEED_MAX)
        return;

    player_spin += delta;
}

void player_update() {
    if(player_speed_x + player_speed_y == 0) return;

    /* Move/turn player. */
    player_x += player_speed_x;
    player_y += player_speed_y;
    player_angle += player_spin;

    /* Check for collision. */
    player_collision();

    /* Apply friction. Stop movement if necessary. */
    player_speed_x *= MOVE_FRICTION;
    player_speed_y *= MOVE_FRICTION;
    player_spin *= TURN_DECEL;

    if(fabs(player_speed_x) + fabs(player_speed_y) < MOVE_FRICTION_STOP)
        player_speed_x = player_speed_y = 0.0;
    if(fabs(player_spin) < TURN_DECEL_STOP)
        player_spin = 0.0;
}
