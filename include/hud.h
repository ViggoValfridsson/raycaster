#ifndef HUD_H
#define HUD_H

#include "common.h"
#include "game.h"
#include <stdint.h>

status_code overlay_hud(const player *player, uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]);

#endif
