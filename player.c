/* player.c */

#include "defs.h"
#include "map.h"
#include "player.h"

double player_x = PLAYER_START_X;
double player_y = PLAYER_START_Y;
double player_angle = PLAYER_START_ANGLE;

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
