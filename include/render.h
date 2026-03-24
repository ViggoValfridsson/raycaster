#ifndef RENDER_H
#define RENDER_H

#include "game.h"
#include "common.h"
#include <stdint.h>

void render_sprites(const game_state *state, uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]);
void raycast(const player *player, int map[MAP_HEIGHT][MAP_WIDTH], uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]);

#endif
