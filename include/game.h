#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#define MAP_HEIGHT 24
#define MAP_WIDTH 24
#define MAX_ENEMY_STATE_FRAMES 4
#define MAX_ENEMIES 24

typedef enum { shotgun } weapon_type;
typedef enum { demon } enemy_type;
typedef enum { idle, attack, hurt, death } enemy_state;

typedef struct {
    int damage;
    int range;
    // Offset indicating how far from right side of screen weapon should appear
    int x_margin;
    weapon_type type;
} weapon;

typedef struct {
    int health;
    int damage;
    enemy_type type;
    enemy_state states;
    int state_frame;
    float pos_x;
    float pos_y;
} enemy;

typedef struct {
    bool is_bob_incrementing;
    short bob_phase;
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
    int enemies_len;
    // Inverted y and x to make array initialization clearer
    int map[MAP_HEIGHT][MAP_WIDTH];
    enemy enemies[MAX_ENEMIES];
} game_state;

typedef enum { EVENT_NONE, EVENT_QUIT, EVENT_MOVE_UP, EVENT_MOVE_DOWN, EVENT_TURN_LEFT, EVENT_TURN_RIGHT } game_event;

void init_game(game_state *state);
void handle_events(game_state *state, const game_event *events, int events_len);

#endif
