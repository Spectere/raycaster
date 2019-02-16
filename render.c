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
static color_t *bg_color;
static Uint64 *bg_color_precalc;

char* color_to_hex(color_t color) {
    static char output[7];
    snprintf(output, 7, "%02X%02X%02X", color.r, color.g, color.b);
    return output;
}

static inline double get_flat_distance(int line) {
    /* Tries to convincingly convert a screen space position to a distance in
     * 3D space. */

    /* Since RENDER_HEIGHT is variable, we need to work out a fixed distance
     * from the top/bottom of the screen first. */
    double ss_dist = (double)line / (double)(RENDER_HEIGHT / 2);
    double render_distance = log(ss_dist * LIGHT_FALLOFF_DISTANCE + (1 + PLAYER_SIZE));

    /* In some libm implementations it's faster to use muiliplication instead
     * of pow() when working with integer powers. */
    int i;
    for(i = 1; i < LIGHT_FLAT_FALLOFF_POWER; i++)
        render_distance *= log(ss_dist * LIGHT_FALLOFF_DISTANCE + (1 + PLAYER_SIZE));

    return render_distance;
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
    int i;

    /* Dimished lighting pre-calculations. */
    bg_color = malloc(sizeof(color_t) * RENDER_HEIGHT);
    bg_color_precalc = malloc(sizeof(Uint64) * RENDER_HEIGHT);
    if(bg_color_precalc == NULL || bg_color == NULL) {
        lprint(ERROR, "Unable to malloc background color array!");
        exit(1);
    }

    /* Figure out what the colors should be for each row of the floor
     * and ceiling. */
    double distance, intensity;
    for(i = 0; i < RENDER_HEIGHT / 2; i++) {
        distance = get_flat_distance(i);
        intensity = log10((distance / LIGHT_FALLOFF_DISTANCE) + 1);
        intensity = intensity > FADE_MAX ? FADE_MAX : intensity;
        BLEND(bg_color[i], ceil_color, fog_color, intensity);
    }

    for(i = RENDER_HEIGHT - 1; i > RENDER_HEIGHT / 2 - 1; i--) {
        distance = get_flat_distance(RENDER_HEIGHT - i - 1);
        intensity = log10((distance / LIGHT_FALLOFF_DISTANCE) + 1);
        intensity = intensity > FADE_MAX ? FADE_MAX : intensity;
        BLEND(bg_color[i], floor_color, fog_color, intensity);
    }

    /* Now precalculate the integer values of each color. */
    for(i = 0; i < RENDER_HEIGHT; i++) {
        Uint32 val = COL_TO_ARGB(bg_color[i]);
        bg_color_precalc[i] = ((Uint64)(val) << 32) + val;
    }
}

void render_quit() {
    free(bg_color);
    free(bg_color_precalc);
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
    int i = 0, row, col;
    for(row = 0; row < RENDER_HEIGHT; row++) {
        for(col = 0; col < RENDER_WIDTH / 2; col++) {
            fill_location[i++] = bg_color_precalc[row];
        }
    }

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
            BLEND(aa_final, bg_color[ry], lit_color, frac_level);
            pixels[POS(rx, ry)] = COL_TO_ARGB(aa_final);
        }

        ry = render_center + (int)line_height;
        if(ry < RENDER_HEIGHT) {
            BLEND(aa_final, bg_color[ry], lit_color, frac_level);
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
