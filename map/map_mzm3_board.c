/* map_mzm3_board.c */

#include <stdio.h>

#include "../log.h"
#include "../player.h"
#include "map.h"
#include "map_mzm3.h"

Uint8 tile_to_char(mzm3_tile_board_t tile) {
	switch(tile.id) {
		case MZX_NORMAL: return 178;
		case MZX_SOLID: return 219;
		case MZX_BREAKAWAY: return 177;
		case MZX_CAVE: return 239;
		case MZX_STAIRS: return 240;

		case MZX_CUSTOM_BLOCK:
		case MZX_CUSTOM_BREAK:
		case MZX_MOVING_WALL_N:
		case MZX_MOVING_WALL_S:
		case MZX_MOVING_WALL_E:
		case MZX_MOVING_WALL_W:
		case MZX_TEXT:
			return tile.param;

		case MZX_SPACE:
		case MZX_LINE:
		case MZX_TREE:
		case MZX_BOULDER:
		case MZX_CRATE:
		case MZX_CUSTOM_PUSH:
		case MZX_BOX:
		case MZX_CUSTOM_BOX:
		case MZX_FAKE:
		case MZX_CARPET:
		case MZX_FLOOR:
		case MZX_TILE:
		case MZX_CUSTOM_FLOOR:
		case MZX_WEB:
		case MZX_THICK_WEB:
		case MZX_STILL_WATER:
		case MZX_WATER_N:
		case MZX_WATER_S:
		case MZX_WATER_E:
		case MZX_WATER_W:
		case MZX_ICE:
		case MZX_LAVA:
		case MZX_CHEST:
		case MZX_GEM:
		case MZX_MAGIC_GEM:
		case MZX_HEALTH:
		case MZX_RING:
		case MZX_POTION:
		case MZX_ENERGIZER:
		case MZX_GOOP:
		case MZX_AMMO:
		case MZX_BOMB:
		case MZX_LIT_BOMB:
		case MZX_EXPLOSION:
		case MZX_KEY:
		case MZX_LOCK:
		case MZX_DOOR:
		case MZX_OPEN_DOOR:
		case MZX_ROTATE_CW:
		case MZX_ROTATE_CCW:
		case MZX_GATE:
		case MZX_OPEN_GATE:
		case MZX_TRANSPORT:
		case MZX_COIN:
		case MZX_POUCH:
		case MZX_PUSHER:
		case MZX_SLIDER_NS:
		case MZX_SLIDER_EW:
		case MZX_LASER:
		case MZX_LASER_GUN:
		case MZX_BULLET:
		case MZX_MISSILE:
		case MZX_FIRE:
		case MZX_FOREST:
		case MZX_LIFE:
		case MZX_WHIRLPOOL_1:
		case MZX_WHIRLPOOL_2:
		case MZX_WHIRLPOOL_3:
		case MZX_WHIRLPOOL_4:
		case MZX_INVIS_WALL:
		case MZX_RICOCHET_PANEL:
		case MZX_RICOCHET:
		case MZX_MINE:
		case MZX_SPIKE:
		case MZX_CUSTOM_HURT:
		case MZX_SHOOTING_FIRE:
		case MZX_SEEKER:
		case MZX_SNAKE:
		case MZX_EYE:
		case MZX_THIEF:
		case MZX_SLIMEBLOB:
		case MZX_RUNNER:
		case MZX_GHOST:
		case MZX_DRAGON:
		case MZX_FISH:
		case MZX_SHARK:
		case MZX_SPIDER:
		case MZX_GOBLIN:
		case MZX_SPITTING_TIGER:
		case MZX_BULLET_GUN:
		case MZX_SPINNING_GUN:
		case MZX_BEAR:
		case MZX_BEAR_CUB:
		case MZX_MISSILE_GUN:
		case MZX_SENSOR:
		case MZX_ROBOT_PUSHABLE:
		case MZX_ROBOT:
		case MZX_SIGN:
		case MZX_SCROLL:
		case MZX_PLAYER:
		default:
			return 0;
	}
}

SDL_bool map_mzm3_board_load(FILE *f, mzm3_header_t header) {
	mzm3_tile_board_t *mzm_data;
	size_t len;
	int i;

	lprint(INFO, "Loading MZM3 board mode dump. Size: (%i, %i)", header.width, header.height);

	map_size_x = header.width;
	map_size_y = header.height;

	len = header.width * header.height;
	mzm_data = malloc(sizeof(mzm3_tile_board_t) * len);
	map_data = malloc(sizeof(map_tile_t) * len);

	fread(mzm_data, sizeof(mzm3_tile_board_t), len, f);

	/* Convert the MZX data into our tile format. */
	for(i = 0; i < len; i++) {
		map_data[i].fg_color = (Uint8)(mzm_data[i].color & 0x0F);
		map_data[i].bg_color = (Uint8)(mzm_data[i].color >> 4);
		map_data[i].texture = tile_to_char(mzm_data[i]);

		/* If the player is on this tile, set their position. */
		if(mzm_data[i].id == MZX_PLAYER) {
			lprint(DEBUG, "Player found at tile #%i", i);

			int y = i / header.width;
			int x = i % header.width;
			player_set_position((double)x + 0.5, (double)y + 0.5);
		}
	}

	free(mzm_data);

	return SDL_TRUE;
}
