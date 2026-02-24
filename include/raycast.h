#ifndef RAYCAST_H
#define RAYCAST_H

#include "game.h"
#include <common.h>
#include <stdint.h>

void raycast(const player *player, int map[MAP_HEIGHT][MAP_WIDTH], uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]);

#endif
