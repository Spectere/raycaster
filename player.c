/* player.c */

#include <errno.h>
#include <math.h>

#include "defs.h"
#include "map/map.h"
#include "player.h"
#include "log.h"

double player_x = -1;
double player_y = -1;
double player_angle = PLAYER_START_ANGLE;

static double player_speed_x = 0.0;
static double player_speed_y = 0.0;
static double player_spin = 0.0;

static double player_lazy_x = -1.0;
static double player_lazy_y = -1.0;

SDL_bool find_valid_player_spawn() {
    int i;

    for(i = 0; i < map_size_x * map_size_y; i++) {
        map_tile_t tile = map_data[i];
        if(tile.texture == 0) {
            int x, y;

            GET_COORD(i, x, y);
            lprint(INFO, "Empty spot found at tile #%i (%i, %i). Placing player there.", i, x, y);
            player_set_position((double)x + 0.5, (double)y + 0.5);

            return SDL_TRUE;
        }
    }

    /* No empty spaces available. */
    return SDL_FALSE;
}

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
    else if(map_data[MAP_POS(player_tile_x1, (int)player_y)].texture != 0)
        player_x = player_tile_x1 + 1 + PLAYER_SIZE;
    else if(player_tile_x2 > map_size_x - 1 || map_data[MAP_POS(player_tile_x2, (int)player_y)].texture != 0)
        player_x = player_tile_x2 - PLAYER_SIZE;

    if(player_y < PLAYER_SIZE)
        player_y = PLAYER_SIZE;
    else if(map_data[MAP_POS((int)player_x, player_tile_y1)].texture != 0)
        player_y = player_tile_y1 + 1 + PLAYER_SIZE;
    else if(player_tile_y2 > map_size_y - 1 || map_data[MAP_POS((int)player_x, player_tile_y2)].texture != 0)
        player_y = player_tile_y2 - PLAYER_SIZE;
}

void player_get_coords_from_string(char *pos, double *x, double *y) {
    char *pos_work, *end;
    double px, py;
    size_t len;
    int i;

    /* Copy the string first so that we can safely manipulate it. */
    len = strlen(pos) + 1;
    pos_work = malloc(sizeof(char) * len);
    strncpy(pos_work, pos, len);

    /* Convert all commas to spaces so that strtod can work its magic. */
    for(i = 0; i < len - 1; i++) {
        if(pos_work[i] == ',')
            pos_work[i] = ' ';
    }

    /* Convert to double, then pass everything along. */
    px = strtod(pos_work, &end);
    py = strtod(end, NULL);
    if(errno == ERANGE) {
        lprint(ERROR, "Invalid player coordinates: %s", pos);
        return;
    }

    *x = px;
    *y = py;
}

SDL_bool player_init() {
    /* Try to set the player's position to what the user wants it to be. */
    if(player_lazy_x >= 0 && player_lazy_y >= 0) {
        lprint(INFO, "Overriding player start position.");
        if(!player_set_position(player_lazy_x, player_lazy_y))
            lprint(WARN, "Invalid player coordinates (%4.2f, %4.2f) specified. Using defaults.",
                    player_lazy_x,
                    player_lazy_y);
    }

    /* Make sure that the player is in a valid starting position. */
    if(player_x < 0 || player_y < 0) {
        /* Oh dear. */
        lprint(WARN, "No player start location found. Searching for an empty spot...");
        if(!find_valid_player_spawn()) {
            lprint(ERROR, "No empty tiles available!");
            return SDL_FALSE;
        }
    }

    return SDL_TRUE;
}

void player_turn(double direction) {
    if(fabs(player_spin) >= TURN_SPEED_MAX)
        return;

    player_spin += TURN_SPEED * direction;
}

SDL_bool player_set_position(double x, double y) {
    if(x >= map_size_x || x < 0 || y >= map_size_y || y < 0) {
        lprint(WARN, "Tried to put player out of bounds! pos: (%2.2f, %2.2f), map size (%i, %i)",
               x, y, map_size_x, map_size_y);
        return SDL_FALSE;
    }

    lprint(INFO, "Setting player position to (%4.2f, %4.2f)", x, y);

    player_x = x;
    player_y = y;

    return SDL_TRUE;
};

void player_set_position_lazy(double x, double y) {
    /* Sets up where we want player_init() to place the player. This is useful
     * for the -s/--start argument, as the map isn't loaded when the command
     * line is parsed. */
    player_lazy_x = x;
    player_lazy_y = y;
}

void player_update() {
    if(player_speed_x + player_speed_y == 0.0 && player_spin == 0.0) return;

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
