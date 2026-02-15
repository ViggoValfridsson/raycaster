#include "game.h"
#include <string.h>

// TODO: Make this cleaner lol
static void init_map(game_state *state) {
    // 0 = air, 1 = wall
    int tmp[MAP_HEIGHT][MAP_WIDTH] = {0};

    // Make a wall in center of northern wall
    for (int x = 40; x <= 60; x++) {
        tmp[0][x] = 1; 
    }

    memcpy(state->map, tmp, sizeof(tmp));
}

void init_game(game_state *state) {
    // Spawn middleish
    state->pos_x = (float)MAP_WIDTH / 2;
    state->pos_y = (float)MAP_HEIGHT / 2;
    // Spawn looking north
    state->dir_x = 0;
    state->dir_y = -1;
    state->plane_x = 0.66f;
    // FOV 66
    state->plane_y = 0;

    init_map(state);
}
