/* render.c */

#include <math.h>

#include "defs.h"
#include "palette.h"
#include "render.h"

const Uint32 CEIL_COLOR = RGB(0x20, 0x20, 0x20);
const Uint32 FLOOR_COLOR = RGB(0x40, 0x40, 0x40);

void render_scene(double view_x, double view_y, double view_angle) {
    double view_angle_rad = DEG2RAD(view_angle);
    double facing_x = sin(view_angle_rad);
    double facing_y = cos(view_angle_rad);

    double camera_plane_x = sin(view_angle_rad + (M_PI / 2));
    double camera_plane_y = cos(view_angle_rad + (M_PI / 2)); /* ~75 degree FOV */

    int rx, ry;
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

        int step_x, step_y, line_height;

        double distance;

        Uint8 hit = 0;
        SDL_bool shade;

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

        if(shade)
            distance = (view_tile_y - view_y + (double)(1 - step_y) / 2) / ray_delta_y;
        else
            distance = (view_tile_x - view_x + (double)(1 - step_x) / 2) / ray_delta_x;

        if(hit == 0) line_height = 0;
        else line_height = (int)(RENDER_HEIGHT / distance);
        for(ry = render_center; ry < render_center + line_height && ry < RENDER_HEIGHT; ry++)
            pixels[POS(rx, ry)] = !shade ? PAL(hit & 0x0F) : PAL_ALT(hit & 0x0F);
        for(ry = render_center - 1; ry > render_center - line_height && ry >= 0; ry--)
            pixels[POS(rx, ry)] = !shade ? PAL(hit & 0x0F) : PAL_ALT(hit & 0x0F);

        /* Draw the floor and ceiling. */
        for(ry = render_center + line_height; ry < RENDER_HEIGHT; ry++)
            pixels[POS(rx, ry)] = FLOOR_COLOR;
        for(ry = render_center - line_height; ry >= 0; ry--)
            pixels[POS(rx, ry)] = CEIL_COLOR;
    }
}
