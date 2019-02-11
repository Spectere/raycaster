/* render.c */

#include <math.h>

#include "defs.h"
#include "palette.h"
#include "render.h"

const Uint8 CEIL_R = 0x20;
const Uint8 CEIL_G = 0x20;
const Uint8 CEIL_B = 0x20;
const Uint8 FLOOR_R = 0x40;
const Uint8 FLOOR_G = 0x40;
const Uint8 FLOOR_B = 0x40;

const Uint32 CEIL_COLOR = RGB(0x20, 0x20, 0x20);
const Uint32 FLOOR_COLOR = RGB(0x40, 0x40, 0x40);

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

        int step_x, step_y;

#ifdef USE_ANTIALIAS
        double line_height, frac_level;
        Uint8 aa_r, aa_g, aa_b;
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

        if(shade)
            distance = (view_tile_y - view_y + (double)(1 - step_y) / 2) / ray_delta_y * WORLD_SCALE * aspect_correction;
        else
            distance = (view_tile_x - view_x + (double)(1 - step_x) / 2) / ray_delta_x * WORLD_SCALE * aspect_correction;

        if(hit == 0) {
            line_height = 0;
        } else {
#ifdef USE_ANTIALIAS
            line_height = (double)RENDER_HEIGHT / distance;
#else
            line_height = (int)(RENDER_HEIGHT / distance);
#endif /* USE_ANTIALIAS */
        }

        for(ry = render_center; ry < render_center + line_height && ry < RENDER_HEIGHT; ry++)
            pixels[POS(rx, ry)] = !shade ? PAL(hit & 0x0F) : PAL_ALT(hit & 0x0F);
        for(ry = render_center - 1; ry > render_center - line_height && ry >= 0; ry--)
            pixels[POS(rx, ry)] = !shade ? PAL(hit & 0x0F) : PAL_ALT(hit & 0x0F);

        /* Draw the floor and ceiling. */
        for(ry = render_center + (int)line_height; ry < RENDER_HEIGHT; ry++)
            pixels[POS(rx, ry)] = FLOOR_COLOR;
        for(ry = render_center - (int)line_height; ry >= 0; ry--)
            pixels[POS(rx, ry)] = CEIL_COLOR;

#ifdef USE_ANTIALIAS
        /* Do a quick anti-aliasing pass using the fractional bit of line_height. This add a bit of
         * overdraw (no more than 2 * RENDER_WIDTH) per frame. */
        frac_level = line_height - (int)line_height;
        if(frac_level == 0) continue;

        ry = render_center - (int)line_height;
        if(ry >= 0) {
            aa_r = (Uint8)((CEIL_R * (1 - frac_level)) + (palette_cga[(hit & 0x0F) * 3] / (shade ? SHADE_FACTOR : 1.0) * frac_level));
            aa_g = (Uint8)((CEIL_G * (1 - frac_level)) + (palette_cga[(hit & 0x0F) * 3 + 1] / (shade ? SHADE_FACTOR : 1.0) * frac_level));
            aa_b = (Uint8)((CEIL_B * (1 - frac_level)) + (palette_cga[(hit & 0x0F) * 3 + 2] / (shade ? SHADE_FACTOR : 1.0) * frac_level));
            pixels[POS(rx, ry)] = RGB(aa_r, aa_g, aa_b);
        }

        ry = render_center + (int)line_height;
        if(ry < RENDER_HEIGHT) {
            aa_r = (Uint8)((FLOOR_R * (1 - frac_level)) + (palette_cga[(hit & 0x0F) * 3] / (shade ? SHADE_FACTOR : 1.0) * frac_level));
            aa_g = (Uint8)((FLOOR_G * (1 - frac_level)) + (palette_cga[(hit & 0x0F) * 3 + 1] / (shade ? SHADE_FACTOR : 1.0) * frac_level));
            aa_b = (Uint8)((FLOOR_B * (1 - frac_level)) + (palette_cga[(hit & 0x0F) * 3 + 2] / (shade ? SHADE_FACTOR : 1.0) * frac_level));
            pixels[POS(rx, ry)] = RGB(aa_r, aa_g, aa_b);
        }
#endif /* USE_ANTIALIAS */
    }
}
