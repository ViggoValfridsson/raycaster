#ifndef GAME_H
#define GAME_H

#define MAP_HEIGHT 100
#define MAP_WIDTH 100

typedef struct {
    float pos_x;
    float pos_y;
    float dir_x;
    float dir_y;
    float plane_x;
    float plane_y;
    int map[MAP_HEIGHT][MAP_WIDTH];
} game_state;

void init_game(game_state *state);

#endif
