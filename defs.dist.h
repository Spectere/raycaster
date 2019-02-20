/* defs.h - Global definitions and constants.
 *
 * Honestly, most of this stuff probably belongs in a config file, but I'm
 * on a roll, yo.*/

#ifndef _DEFS_DIST_H
#define _DEFS_DIST_H

#include <math.h>

/* Maximum length of the log format string, including the severity tag. */
#define LOG_LEN 160

/* Math stuff. */
#define DEG2RAD(angle) (angle * (M_PI / 180))

/*
 * User-definable stuff below! These should be overridden using defs.h.
 *
 * Ideally, this stuff should eventually make it into a config parameter and/or
 * command-line parameters.
 */

/* Supported optional features. */
#ifndef NO_ANTIALIAS
#define USE_ANTIALIAS
#endif /* NO_ANTIALIAS */

/* The size of the pixmap. */
#ifndef RENDER_WIDTH
#define RENDER_WIDTH 320
#endif

#ifndef RENDER_HEIGHT
#define RENDER_HEIGHT 240
#endif

/* The SDL window size. */
#ifndef RENDER_SCALE
#define RENDER_SCALE 3
#endif

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH (RENDER_WIDTH * RENDER_SCALE)
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT (RENDER_HEIGHT * RENDER_SCALE)
#endif

#if !defined(WINDOW_SCALING_LINEAR) && !defined(WINDOW_SCALING_NEAREST)
#define WINDOW_SCALING_AUTO
#endif

/* The world scale. Lower this to make things feel more claustrophobic.
 * 4x feels roughly the same size as Wolf3D. */
#ifndef WORLD_SCALE
#define WORLD_SCALE 3
#endif

/* Move/turn speeds. */
#ifndef MOVE_FRICTION
#define MOVE_FRICTION 0.5
#endif

#ifndef MOVE_FRICTION_STOP
#define MOVE_FRICTION_STOP 0.01
#endif

#ifndef MOVE_SPEED
#define MOVE_SPEED 0.05
#endif

#ifndef MOVE_SPEED_MAX
#define MOVE_SPEED_MAX 0.1
#endif

#ifndef TURN_DECEL
#define TURN_DECEL 0.2
#endif

#ifndef TURN_DECEL_STOP
#define TURN_DECEL_STOP 0.01
#endif

#ifndef TURN_SPEED
#define TURN_SPEED 1.75
#endif

#ifndef TURN_SPEED_MAX
#define TURN_SPEED_MAX 3.5
#endif

/* Player data. */
#ifndef PLAYER_START_ANGLE
#define PLAYER_START_ANGLE 90
#endif

#ifndef PLAYER_SIZE
#define PLAYER_SIZE 0.35
#endif

/* Shading/lighting. */
#ifndef SHADE_FACTOR
#define SHADE_FACTOR 1.5
#endif

#ifndef LIGHT_FALLOFF_DISTANCE
#define LIGHT_FALLOFF_DISTANCE 12
#endif

#ifndef LIGHT_FLAT_FALLOFF_POWER
#define LIGHT_FLAT_FALLOFF_POWER 3
#endif

#ifndef FADE_MAX
#define FADE_MAX 0.5
#endif

#endif /* _DEFS_DIST_H */
