#include "common.h"
#include "game.h"
#include <stdint.h>

status_code raycast(game_state *state, uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]) {
    status_code return_value = STATUS_RENDER_ERROR;

    // Loop through each vertical strip of the screen
    for (int x = 0; x < RENDER_WIDTH; x++) {

    }

    return_value = STATUS_SUCCESS;
exit:
    return return_value;
}
