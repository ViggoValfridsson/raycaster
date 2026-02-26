#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#define MAP_HEIGHT 24
#define MAP_WIDTH 24

typedef enum {
    shotgun
} weapon_type;

typedef struct{
    int damage;
    int range;
    // Offset indicating how far from right side of screen weapon should appear
    int x_margin;
    weapon_type type;
} weapon;

typedef struct {
    short health;
    float pos_x;
    float pos_y;
    float dir_x;
    float dir_y;
    float plane_x;
    float plane_y;
    weapon weapon;
} player;

typedef struct {
    bool is_running;
    player player;
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
