#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_events.h>
#include <stdbool.h>

#define MAP_HEIGHT 24
#define MAP_WIDTH 24

typedef struct {
    bool is_running;
    float pos_x;
    float pos_y;
    float dir_x;
    float dir_y;
    float plane_x;
    float plane_y;
    int map[MAP_WIDTH][MAP_HEIGHT];
} game_state;

void init_game(game_state *state);
void handle_events(game_state *state);

#endif
