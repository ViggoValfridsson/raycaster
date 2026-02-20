#include "common.h"
#include "game.h"
#include "sdl.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

static int half_brightness(int color) {
    return (color & 0xFF000000) | ((color & 0x00FEFEFE) >> 1);
}

static int get_wall_color(const game_state *state, int map_x, int map_y, bool is_side) {
    int color;

    switch (state->map[map_y][map_x]) {
    case 1:
        color = ARGB_RED;
        break;
    case 2:
        color = ARGB_GREEN;
        break;
    case 3:
        color = ARGB_BLUE;
        break;
    default:
        color = ARGB_YELLOW;
        break;
    }

    if (is_side)
        color = half_brightness(color);

    return color;
}

void draw_vertical_slice(uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH], int render_x, int color, int draw_start,
                         int draw_end) {
    for (int y = 0; y < RENDER_HEIGHT; y++) {
        int cell_color = y >= draw_start && y < draw_end ? color : ARGB_BLACK;

        frame_buffer[y][render_x] = cell_color;
    }
}

void raycast(const game_state *state, uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]) {
    for (int x = 0; x < RENDER_WIDTH; x++) {
        double camera_x = 2 * x / (double)RENDER_WIDTH - 1;
        double ray_dir_x = state->dir_x + state->plane_x * camera_x;
        double ray_dir_y = state->dir_y + state->plane_y * camera_x;

        // Current ray cell
        int map_x = (int)state->pos_x;
        int map_y = (int)state->pos_y;

        // length of ray from current position to next side
        double side_dist_x;
        double side_dist_y;

        // length of ray x side to next x side or y side to next y side. 1e30 to represent inifinity and avoid divide by
        // zero errors
        double delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
        double delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
        // Fisheye corrected dist
        double perpendicular_wall_dist;

        // direction to step in
        int step_x;
        int step_y;

        bool is_hit = false;
        bool is_side = false;

        if (ray_dir_x < 0) {
            step_x = -1;
            side_dist_x = (state->pos_x - map_x) * delta_dist_x;
        } else {
            step_x = 1;
            side_dist_x = (map_x + 1 - state->pos_x) * delta_dist_x;
        }

        if (ray_dir_y < 0) {
            step_y = -1;
            side_dist_y = (state->pos_y - map_y) * delta_dist_y;
        } else {
            step_y = 1;
            side_dist_y = (map_y + 1 - state->pos_y) * delta_dist_y;
        }

        while (!is_hit) {
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                is_side = false;
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                is_side = true;
            }

            if (state->map[map_y][map_x])
                is_hit = true;
        }

        if (is_side)
            perpendicular_wall_dist = (side_dist_y - delta_dist_y);
        else
            perpendicular_wall_dist = (side_dist_x - delta_dist_x);

        int line_height = (int)(RENDER_HEIGHT / perpendicular_wall_dist);

        int draw_start = (-line_height / 2) + (RENDER_HEIGHT / 2);
        if (draw_start < 0)
            draw_start = 0;

        int draw_end = line_height / 2 + RENDER_HEIGHT / 2;
        if (draw_end >= RENDER_HEIGHT)
            draw_end = RENDER_HEIGHT - 1;

        int color = get_wall_color(state, map_x, map_y, is_side);
        draw_vertical_slice(frame_buffer, x, color, draw_start, draw_end);
    }
}
