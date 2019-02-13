/* map_mzm3.h */

#ifndef _MAP_MZM3_H
#define _MAP_MZM3_H

#include <SDL.h>

#include "map.h"

typedef enum {
	MZM3_BOARD = 0x00,
	MZM3_LAYER = 0x01
} mzm3_storage_mode;

typedef enum {
	MZX_SPACE = 0x00,
	MZX_NORMAL = 0x01,
	MZX_SOLID = 0x02,
	MZX_TREE = 0x03,
	MZX_LINE = 0x04,
	MZX_CUSTOM_BLOCK = 0x05,
	MZX_BREAKAWAY = 0x06,
	MZX_CUSTOM_BREAK = 0x07,
	MZX_BOULDER = 0x08,
	MZX_CRATE = 0x09,
	MZX_CUSTOM_PUSH = 0x0A,
	MZX_BOX = 0x0B,
	MZX_CUSTOM_BOX = 0x0C,
	MZX_FAKE = 0x0D,
	MZX_CARPET = 0x0E,
	MZX_FLOOR = 0x0F,
	MZX_TILE = 0x10,
	MZX_CUSTOM_FLOOR = 0x11,
	MZX_WEB = 0x12,
	MZX_THICK_WEB = 0x13,
	MZX_STILL_WATER = 0x14,
	MZX_WATER_N = 0x15,
	MZX_WATER_S = 0x16,
	MZX_WATER_E = 0x17,
	MZX_WATER_W = 0x18,
	MZX_ICE = 0x19,
	MZX_LAVA = 0x1A,
	MZX_CHEST = 0x1B,
	MZX_GEM = 0x1C,
	MZX_MAGIC_GEM = 0x1D,
	MZX_HEALTH = 0x1E,
	MZX_RING = 0x1F,
	MZX_POTION = 0x20,
	MZX_ENERGIZER = 0x21,
	MZX_GOOP = 0x22,
	MZX_AMMO = 0x23,
	MZX_BOMB = 0x24,
	MZX_LIT_BOMB = 0x25,
	MZX_EXPLOSION = 0x26,
	MZX_KEY = 0x27,
	MZX_LOCK = 0x28,
	MZX_DOOR = 0x29,
	MZX_OPEN_DOOR = 0x2A,
	MZX_STAIRS = 0x2B,
	MZX_CAVE = 0x2C,
	MZX_ROTATE_CW = 0x2D,
	MZX_ROTATE_CCW = 0x2E,
	MZX_GATE = 0x2F,
	MZX_OPEN_GATE = 0x30,
	MZX_TRANSPORT = 0x31,
	MZX_COIN = 0x32,
	MZX_MOVING_WALL_N = 0x33,
	MZX_MOVING_WALL_S = 0x34,
	MZX_MOVING_WALL_E = 0x35,
	MZX_MOVING_WALL_W = 0x36,
	MZX_POUCH = 0x37,
	MZX_PUSHER = 0x38,
	MZX_SLIDER_NS = 0x39,
	MZX_SLIDER_EW = 0x3A,
	MZX_LASER = 0x3B,
	MZX_LASER_GUN = 0x3C,
	MZX_BULLET = 0x3D,
	MZX_MISSILE = 0x3E,
	MZX_FIRE = 0x3F,
	MZX_FOREST = 0x41,
	MZX_LIFE = 0x42,
	MZX_WHIRLPOOL_1 = 0x43,
	MZX_WHIRLPOOL_2 = 0x44,
	MZX_WHIRLPOOL_3 = 0x45,
	MZX_WHIRLPOOL_4 = 0x46,
	MZX_INVIS_WALL = 0x47,
	MZX_RICOCHET_PANEL = 0x48,
	MZX_RICOCHET = 0x49,
	MZX_MINE = 0x4A,
	MZX_SPIKE = 0x4B,
	MZX_CUSTOM_HURT = 0x4C,
	MZX_TEXT = 0x4D,
	MZX_SHOOTING_FIRE = 0x4E,
	MZX_SEEKER = 0x4F,
	MZX_SNAKE = 0x50,
	MZX_EYE = 0x51,
	MZX_THIEF = 0x52,
	MZX_SLIMEBLOB = 0x53,
	MZX_RUNNER = 0x54,
	MZX_GHOST = 0x55,
	MZX_DRAGON = 0x56,
	MZX_FISH = 0x57,
	MZX_SHARK = 0x58,
	MZX_SPIDER = 0x59,
	MZX_GOBLIN = 0x5A,
	MZX_SPITTING_TIGER = 0x5B,
	MZX_BULLET_GUN = 0x5C,
	MZX_SPINNING_GUN = 0x5D,
	MZX_BEAR = 0x5E,
	MZX_BEAR_CUB = 0x5F,
	MZX_MISSILE_GUN = 0x61,
	MZX_SENSOR = 0x7A,
	MZX_ROBOT_PUSHABLE = 0x7B,
	MZX_ROBOT = 0x7C,
	MZX_SIGN = 0x7D,
	MZX_SCROLL = 0x7E,
	MZX_PLAYER = 0x7F
} mzx_object;

typedef struct mzm3_header {
	char magic[4];
	Uint16 width;
	Uint16 height;
	Uint32 robot_data;
	Uint8 robot_count;
	Uint8 storage_mode;
	Uint8 has_save_data;
	Uint16 world_version;
	Uint8 reserved[2];
} mzm3_header_t;

typedef struct mzm3_tile_board {
	Uint8 id;
	Uint8 param;
	Uint8 color;
	Uint8 under_id;
	Uint8 under_param;
	Uint8 under_color;
} mzm3_tile_board_t;

typedef struct mzm3_tile_layer {
	Uint8 ch;
	Uint8 color;
} mzm3_tile_layer_t;

SDL_bool map_mzm3_board_load(FILE *f, mzm3_header_t header);
SDL_bool map_mzm3_check(FILE *f);
SDL_bool map_mzm3_load(FILE *f);

#endif /* _MAP_MZM3_H */
