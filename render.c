/* render.c */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "log.h"
#include "palette/palette.h"
#include "render.h"

static color_t ceil_color = { 0xFF, 0x20, 0x20, 0x20 };
static color_t floor_color = { 0xFF, 0x40, 0x40, 0x40 };
static color_t fog_color = { 0xFF, 0x00, 0x00, 0x00 };

static double aspect_correction;
static Uint64 ceil_color_precalc, floor_color_precalc;

char* color_to_hex(color_t color) {
    static char output[7];
    snprintf(output, 7, "%02X%02X%02X", color.r, color.g, color.b);
    return output;
}

void hex_to_color(char *hex, color_t *color) {
    int offset = 0, len = (int)strlen(hex);
    char component[3] = "\0\0\0";

    /* Accept input in both RRGGBB and #RRGGBB formats. */
    if(len != 6 && !(len == 7 && hex[0] == '#'))
        return;

    if(hex[0] == '#') offset++;

    strncpy(component, hex + offset, 2);
    color->r = (Uint8)strtol(component, NULL, 16);

    strncpy(component, hex + offset + 2, 2);
    color->g = (Uint8)strtol(component, NULL, 16);

    strncpy(component, hex + offset + 4, 2);
    color->b = (Uint8)strtol(component, NULL, 16);
}

void render_init() {
    aspect_correction = ((double)RENDER_HEIGHT / (double)RENDER_WIDTH);

    ceil_color_precalc = ((Uint64)COL_TO_ARGB(ceil_color) << 32) + COL_TO_ARGB(ceil_color);
    floor_color_precalc = ((Uint64)COL_TO_ARGB(floor_color) << 32) + COL_TO_ARGB(floor_color);
}

void render_scene(double view_x, double view_y, double view_angle) {
    double view_angle_rad = DEG2RAD(view_angle);
    double facing_x = sin(view_angle_rad);
    double facing_y = -cos(view_angle_rad);

    double camera_plane_x = sin(view_angle_rad + (M_PI / 2));
    double camera_plane_y = -cos(view_angle_rad + (M_PI / 2));

    int rx, ry, render_start;
    int render_center = RENDER_HEIGHT / 2;

    Uint64 *fill_location = (Uint64*)pixels;
    int i, fill_size = render_center * RENDER_WIDTH / 2;
    for(i = 0; i < fill_size; i++)
        fill_location[i] = ceil_color_precalc;
    for(; i < RENDER_WIDTH * RENDER_HEIGHT / 2; i++)
        fill_location[i] = floor_color_precalc;

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
        color_t wall_color, lit_color;
        double intensity;

#ifdef USE_ANTIALIAS
        double line_height, frac_level;
        color_t aa_final;
#else
        int line_height;
#endif /* USE_ANTIALIAS */

        double distance;

        map_tile_t *hit = NULL;
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
        while(hit == NULL || hit->texture == 0) {
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
            if(view_tile_x >= map_size_x || view_tile_x < 0
            || view_tile_y >= map_size_y || view_tile_y < 0) {
                hit = NULL;
                break;
            }

            hit = &map_data[(view_tile_y * map_size_x) + view_tile_x];
        }

        if(hit == NULL) {
            line_height = 0;
        } else {
            if(shade) {
                distance = (view_tile_y - view_y + (double)(1 - step_y) / 2) / ray_delta_y * WORLD_SCALE * aspect_correction;
                SHADE_COL(palette[hit->fg_color], wall_color);
            } else {
                distance = (view_tile_x - view_x + (double)(1 - step_x) / 2) / ray_delta_x * WORLD_SCALE * aspect_correction;
                wall_color = palette[hit->fg_color];
            }

            /* Add mood. :) */
            if(distance > 0) {
                /* Multiplication is faster than pow() for calculating squares in
                 * some libm implementations. */
                intensity = log10((distance / LIGHT_FALLOFF_DISTANCE) + 1);
                intensity = intensity > FADE_MAX ? FADE_MAX : intensity;
            } else {
                intensity = 0.0;
            }
            BLEND(lit_color, wall_color, fog_color, intensity);

#ifdef USE_ANTIALIAS
            line_height = (double)RENDER_HEIGHT / distance;
#else
            line_height = (int)(RENDER_HEIGHT / distance);
#endif /* USE_ANTIALIAS */
        }

        render_start = render_center - (int)line_height + 1;
        for(ry = render_start >= 0 ? render_start : 0; ry < render_center + line_height && ry < RENDER_HEIGHT; ry++) {
            pixels[POS(rx, ry)] = COL_TO_ARGB(lit_color);
        }

#ifdef USE_ANTIALIAS
        /* Do a quick anti-aliasing pass using the fractional bit of line_height. This adds a bit of
         * overdraw (no more than 2 * RENDER_WIDTH) per frame. */
        frac_level = line_height - (int)line_height;
        if(frac_level == 0) continue;

        ry = render_center - (int)line_height;

        if(ry >= 0) {
            BLEND(aa_final, ceil_color, lit_color, frac_level);
            pixels[POS(rx, ry)] = COL_TO_ARGB(aa_final);
        }

        ry = render_center + (int)line_height;
        if(ry < RENDER_HEIGHT) {
            BLEND(aa_final, floor_color, lit_color, frac_level);
            pixels[POS(rx, ry)] = COL_TO_ARGB(aa_final);
        }
#endif /* USE_ANTIALIAS */
    }
}

void render_set_ceiling_color(char *hex) {
    hex_to_color(hex, &ceil_color);
    lprint(INFO, "Ceiling color set to %s", color_to_hex(ceil_color));
}

void render_set_floor_color(char *hex) {
    hex_to_color(hex, &floor_color);
    lprint(INFO, "Floor color set to %s", color_to_hex(floor_color));
}
