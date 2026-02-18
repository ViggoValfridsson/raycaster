#include "common.h"
#include "game.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct ray {
    bool is_hit;
} ray;

status_code raycast(const game_state *state, const uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]) {
    status_code return_value = STATUS_RENDER_ERROR;

    for (int x = 0; x < RENDER_WIDTH; x++) {
        double camera_x = 2 * x / (double)RENDER_WIDTH - 1;
        double ray_dir_y = state->dir_x + state->plane_x * camera_x;
        double ray_dir_x = state->dir_y + state->plane_y * camera_x;

        // Current ray cell
        int map_x = (int)state->pos_x;
        int map_y = (int)state->pos_y;

        // length of ray from current position to next side
        double side_dist_x;
        double side_dist_y;

        // length of ray x side to next x side or y side to next y side
        double delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
        double delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
        // Fisheye corrected dist
        double peripheral_wall_dist;

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
        }
    }

    return_value = STATUS_SUCCESS;
exit:
    return return_value;
}
