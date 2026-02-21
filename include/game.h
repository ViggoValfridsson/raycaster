#ifndef GAME_H
#define GAME_H

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
    // Inverted y and x to make array initialization clearer
    int map[MAP_HEIGHT][MAP_WIDTH];
} game_state;

typedef enum {
    EVENT_NONE,
    EVENT_QUIT,
    EVENT_MOVE_UP,
    EVENT_MOVE_DOWN,
    EVENT_MOVE_LEFT,
    EVENT_MOVE_RIGHT 
} game_event;

void init_game(game_state *state);
void handle_events(game_state *state, const game_event *events, int events_len);

#endif
