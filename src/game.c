#include "game.h"
#include <math.h>
#include <string.h>

#define ROT_SPEED 0.04f
#define MOV_SPEED 0.12f

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

static weapon create_shotgun() {
    weapon gun = {.damage = 10, .range = 10, .type = shotgun, .x_margin = 50};
    return gun;
}

void init_game(game_state *state) {
    state->is_running = true;

    // Spawn middle
    state->player.pos_x = (float)MAP_WIDTH / 2;
    state->player.pos_y = (float)MAP_HEIGHT / 2;

    // looking north
    state->player.dir_x = 0;
    state->player.dir_y = -1;

    // 66 fov
    state->player.plane_x = 0.66f;
    state->player.plane_y = 0;

    state->player.weapon = create_shotgun();

    init_map(state);
}

static void move(game_state *state, float move_dir_y, float move_dir_x) {
    const float margin = 0.3f;

    float desired_pos_y = state->player.pos_y + move_dir_y * MOV_SPEED;
    float desired_pos_x = state->player.pos_x + move_dir_x * MOV_SPEED;

    if (!state->map[(int)(desired_pos_y + margin)][(int)state->player.pos_x] &&
        !state->map[(int)(desired_pos_y - margin)][(int)state->player.pos_x]) {
        state->player.pos_y = desired_pos_y;
    }

    if (!state->map[(int)state->player.pos_y][(int)(desired_pos_x + margin)] &&
        !state->map[(int)state->player.pos_y][(int)(desired_pos_x - margin)]) {
        state->player.pos_x = desired_pos_x;
    }
}

static void turn(game_state *state, direction direction) {
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

    double old_dir_x = state->player.dir_x;
    state->player.dir_x = state->player.dir_x * cos(rot_speed) - state->player.dir_y * sin(rot_speed);
    state->player.dir_y = old_dir_x * sin(rot_speed) + state->player.dir_y * cos(rot_speed);

    double old_plane_x = state->player.plane_x;
    state->player.plane_x = state->player.plane_x * cos(rot_speed) - state->player.plane_y * sin(rot_speed);
    state->player.plane_y = old_plane_x * sin(rot_speed) + state->player.plane_y * cos(rot_speed);
}

void handle_events(game_state *state, const game_event *events, int events_len) {
    for (int i = 0; i < events_len; i++) {
        switch (events[i]) {
        case EVENT_QUIT:
            state->is_running = false;
            break;
        case EVENT_MOVE_UP:
            move(state, state->player.dir_y, state->player.dir_x);
            break;
        case EVENT_MOVE_DOWN:
            move(state, -state->player.dir_y, -state->player.dir_x);
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
