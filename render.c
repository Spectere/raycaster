/* render.c */

#include <math.h>

#include "defs.h"
#include "palette/palette.h"
#include "render.h"

const color_t CEIL_COLOR = { 0xFF, 0x20, 0x20, 0x20 };
const color_t FLOOR_COLOR = { 0xFF, 0x40, 0x40, 0x40 };

static double aspect_correction;

void render_init() {
    aspect_correction = ((double)RENDER_HEIGHT / (double)RENDER_WIDTH);
}

void render_scene(double view_x, double view_y, double view_angle) {
    double view_angle_rad = DEG2RAD(view_angle);
    double facing_x = sin(view_angle_rad);
    double facing_y = -cos(view_angle_rad);

    double camera_plane_x = sin(view_angle_rad + (M_PI / 2));
    double camera_plane_y = -cos(view_angle_rad + (M_PI / 2));

    int rx, ry, render_start;
    int render_center = RENDER_HEIGHT / 2;

    /* Cast one ray for each vertical strip. */
    for(rx = 0; rx < RENDER_WIDTH; rx++) {
        double camera_x = 2 * rx / (double)RENDER_WIDTH - 1;
        double ray_delta_x = facing_x + camera_plane_x * camera_x;
        double ray_delta_y = facing_y + camera_plane_y * camera_x;

        int view_tile_x = (int)view_x;
        int view_tile_y = (int)view_y;

        double side_distance_x, side_distance_y;

        double delta_x = fabs(1 / ray_delta_x);
        double delta_y = fabs(1 / ray_delta_y);

        int step_x, step_y;
        color_t wall_color;

#ifdef USE_ANTIALIAS
        double line_height, frac_level;
        color_t aa_final;
#else
        int line_height;
#endif /* USE_ANTIALIAS */

        double distance;

        Uint8 hit = 0;
        SDL_bool shade = SDL_FALSE;

        if(ray_delta_x > 0) {
            step_x = 1;
            side_distance_x = (view_tile_x + 1.0 - view_x) * delta_x;
        } else {
            step_x = -1;
            side_distance_x = (view_x - view_tile_x) * delta_x;
        }

        if(ray_delta_y > 0) {
            step_y = 1;
            side_distance_y = (view_tile_y + 1.0 - view_y) * delta_y;
        } else {
            step_y = -1;
            side_distance_y = (view_y - view_tile_y) * delta_y;
        }

        /* DDA--where all the fun ray stuff happens */
        while(hit == 0) {
            if(side_distance_x > side_distance_y) {
                side_distance_y += delta_y;
                view_tile_y += step_y;
                shade = SDL_TRUE;
            } else {
                side_distance_x += delta_x;
                view_tile_x += step_x;
                shade = SDL_FALSE;
            }

            /* Bail out if the ray escapes the map boundary. */
            if(view_tile_x >= MAP_SIZE_X || view_tile_x < 0
            || view_tile_y >= MAP_SIZE_Y || view_tile_y < 0)
                break;

            hit = map_data[(view_tile_y * MAP_SIZE_X) + view_tile_x];
        }

        if(shade) {
            distance = (view_tile_y - view_y + (double) (1 - step_y) / 2) / ray_delta_y * WORLD_SCALE * aspect_correction;
            SHADE_COL(palette_cga[hit & 0x0F], wall_color);
        } else {
            distance = (view_tile_x - view_x + (double) (1 - step_x) / 2) / ray_delta_x * WORLD_SCALE * aspect_correction;
            wall_color = palette_cga[hit & 0x0F];
        }

        if(hit == 0) {
            line_height = 0;
        } else {
#ifdef USE_ANTIALIAS
            line_height = (double)RENDER_HEIGHT / distance;
#else
            line_height = (int)(RENDER_HEIGHT / distance);
#endif /* USE_ANTIALIAS */
        }

        render_start = render_center - (int)line_height;
        for(ry = render_start >= 0 ? render_start : 0; ry < render_center + line_height && ry < RENDER_HEIGHT; ry++)
            pixels[POS(rx, ry)] = COL_TO_ARGB(wall_color);

        /* Draw the floor and ceiling. */
        for(ry = render_center + (int)line_height; ry < RENDER_HEIGHT; ry++)
            pixels[POS(rx, ry)] = COL_TO_ARGB(FLOOR_COLOR);
        for(ry = render_center - (int)line_height; ry >= 0; ry--)
            pixels[POS(rx, ry)] = COL_TO_ARGB(CEIL_COLOR);

#ifdef USE_ANTIALIAS
        /* Do a quick anti-aliasing pass using the fractional bit of line_height. This add a bit of
         * overdraw (no more than 2 * RENDER_WIDTH) per frame. */
        frac_level = line_height - (int)line_height;
        if(frac_level == 0) continue;

        ry = render_center - (int)line_height;

        if(ry >= 0) {
            BLEND(aa_final, CEIL_COLOR, wall_color, frac_level);
            pixels[POS(rx, ry)] = COL_TO_ARGB(aa_final);
        }

        ry = render_center + (int)line_height;
        if(ry < RENDER_HEIGHT) {
            BLEND(aa_final, FLOOR_COLOR, wall_color, frac_level);
            pixels[POS(rx, ry)] = COL_TO_ARGB(aa_final);
        }
#endif /* USE_ANTIALIAS */
    }
}
