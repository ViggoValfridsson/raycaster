#ifndef RAYCAST_H
#define RAYCAST_H

#include "game.h"
#include <common.h>
#include <stdint.h>

status_code raycast(game_state *state, uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]);

#endif
