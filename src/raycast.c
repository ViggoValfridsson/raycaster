#include "common.h"
#include "game.h"
#include <stdint.h>

status_code raycast(game_state *state, uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]) {
    static int changing = 0;
    status_code return_value = STATUS_RENDER_ERROR;
    changing = changing == 0 ? 1 : 0;

    int checker_size = changing == 1 ? 16 : 8;

    for (int y = 0; y < RENDER_HEIGHT; y++) {
        for (int x = 0; x < RENDER_WIDTH; x++) {
            int checker = ((x / checker_size) + (y / checker_size)) % 2;
            frame_buffer[y][x] = checker ? 0xFFFF0000 : 0xFF0000FF;
        }
    }
    
    return_value = STATUS_SUCCESS;
    
exit:
    return return_value;
}
