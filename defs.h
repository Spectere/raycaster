/* defs.h - Global definitions and constants. */

#ifndef _DEFS_H
#define _DEFS_H

#include <math.h>

/* The size of the pixmap. */
#define RENDER_WIDTH 320
#define RENDER_HEIGHT 240

/* The SDL window size. */
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720

/* Maximum length of the log format string, including the severity tag. */
#define LOG_LEN 160

/* Move/turn speeds. */
#define MOVE_SPEED 0.125
#define TURN_SPEED 2.5

/* Player starting position. */
#define PLAYER_START_X 8
#define PLAYER_START_Y 8
#define PLAYER_START_ANGLE 0.0

/* Math stuff. */
#define DEG2RAD(angle) (angle * (M_PI / 180))

#endif /* _DEFS_H */
