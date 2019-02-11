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
#define MOVE_FRICTION 0.5
#define MOVE_FRICTION_STOP 0.01
#define MOVE_SPEED 0.05
#define MOVE_SPEED_MAX 0.1
#define TURN_DECEL 0.75
#define TURN_DECEL_STOP 0.01
#define TURN_SPEED 1.75
#define TURN_SPEED_MAX 3.5

/* Player starting position. */
#define PLAYER_START_X 8.5
#define PLAYER_START_Y 8.5
#define PLAYER_START_ANGLE 90
#define PLAYER_SIZE 0.5

/* Math stuff. */
#define DEG2RAD(angle) (angle * (M_PI / 180))

#endif /* _DEFS_H */
