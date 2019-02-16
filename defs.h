/* defs.h - Global definitions and constants.
 *
 * Honestly, most of this stuff probably belongs in a config file, but I'm
 * on a roll, yo.*/

#ifndef _DEFS_H
#define _DEFS_H

#include <math.h>

/* Optional features. */
#define USE_ANTIALIAS

/* The size of the pixmap. */
#define RENDER_WIDTH 1920
#define RENDER_HEIGHT 1080

/* The SDL window size. */
#define RENDER_SCALE 1
#define WINDOW_WIDTH (RENDER_WIDTH * RENDER_SCALE)
#define WINDOW_HEIGHT (RENDER_HEIGHT * RENDER_SCALE)
/*#define WINDOW_SCALING_NEAREST*/
/*#define WINDOW_SCALING_LINEAR*/
#define WINDOW_SCALING_AUTO

/* The world scale. Lower this to make things feel more claustrophobic.
 * 4x feels roughly the same size as Wolf3D. */
#define WORLD_SCALE 3

/* Maximum length of the log format string, including the severity tag. */
#define LOG_LEN 160

/* Move/turn speeds. */
#define MOVE_FRICTION 0.5
#define MOVE_FRICTION_STOP 0.01
#define MOVE_SPEED 0.05
#define MOVE_SPEED_MAX 0.1
#define TURN_DECEL 0.2
#define TURN_DECEL_STOP 0.01
#define TURN_SPEED 1.75
#define TURN_SPEED_MAX 3.5

/* Player data. */
#define PLAYER_START_ANGLE 90
#define PLAYER_SIZE 0.35

/* Math stuff. */
#define DEG2RAD(angle) (angle * (M_PI / 180))

/* Shading/lighting. */
#define SHADE_FACTOR 1.5
#define LIGHT_FALLOFF_DISTANCE 12
#define LIGHT_FLAT_FALLOFF_POWER 3
#define FADE_MAX 0.5

#endif /* _DEFS_H */
