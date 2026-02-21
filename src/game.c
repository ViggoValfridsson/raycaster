#include "game.h"
#include <math.h>
#include <string.h>

#define ROT_SPEED 0.1f

typedef enum { RIGHT, LEFT } direction;

static void init_map(game_state *state) {
    int tmp[MAP_HEIGHT][MAP_WIDTH] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    memcpy(state->map, tmp, sizeof(tmp));
}

void init_game(game_state *state) {
    state->is_running = true;

    // Spawn middle
    state->pos_x = (float)MAP_WIDTH / 2;
    state->pos_y = (float)MAP_HEIGHT / 2;

    // looking north
    state->dir_x = 0;
    state->dir_y = -1;

    // 66 fov
    state->plane_x = 0.66f;
    state->plane_y = 0;

    init_map(state);
}

void move_unless_collission(game_state *state, float move_dir_y, float move_dir_x) {
    const float margin = 0.3f;

    float desired_pos_y = state->pos_y + move_dir_y;
    float desired_pos_x = state->pos_x + move_dir_x;

    if (!state->map[(int)(desired_pos_y + margin)][(int)state->pos_x] &&
        !state->map[(int)(desired_pos_y - margin)][(int)state->pos_x]) {
        state->pos_y = desired_pos_y;
    }

    if (!state->map[(int)state->pos_y][(int)(desired_pos_x + margin)] &&
        !state->map[(int)state->pos_y][(int)(desired_pos_x - margin)]) {
        state->pos_x = desired_pos_x;
    }
}

void turn(game_state *state, direction direction) {
    float rot_speed;

    switch (direction) {
    case RIGHT:
        rot_speed = ROT_SPEED;
        break;
    case LEFT:
        rot_speed = -ROT_SPEED;
        break;
    default:
        rot_speed = ROT_SPEED;
    }

    double old_dir_x = state->dir_x;
    state->dir_x = state->dir_x * cos(rot_speed) - state->dir_y * sin(rot_speed);
    state->dir_y = old_dir_x * sin(rot_speed) + state->dir_y * cos(rot_speed);

    double old_plane_x = state->plane_x;
    state->plane_x = state->plane_x * cos(rot_speed) - state->plane_y * sin(rot_speed);
    state->plane_y = old_plane_x * sin(rot_speed) + state->plane_y * cos(rot_speed);
}

void handle_events(game_state *state, const game_event *events, int events_len) {
    for (int i = 0; i < events_len; i++) {
        switch (events[i]) {
        case EVENT_QUIT:
            state->is_running = false;
            break;
        case EVENT_MOVE_UP:
            move_unless_collission(state, state->dir_y, state->dir_x);
            break;
        case EVENT_MOVE_DOWN:
            move_unless_collission(state, -state->dir_y, -state->dir_x);
            break;
        case EVENT_MOVE_RIGHT:
            turn(state, RIGHT);
            break;
        case EVENT_MOVE_LEFT:
            turn(state, LEFT);
            break;
        case EVENT_NONE:
        default:
            break;
        }
    }
}
