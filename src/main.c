#include "common.h"
#include "game.h"
#include "raycast.h"
#include "sdl.h"

int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    status_code return_value = STATUS_ERROR;

    return_value = sdl_create_context("Raycaster", &window, &renderer, &texture);
    if (return_value != STATUS_SUCCESS)
        goto exit;

    game_state state;
    init_game(&state);

    uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH];

    while (state.is_running) {
        handle_events(&state);
        raycast(&state, frame_buffer);

        return_value = render_frame(renderer, texture, frame_buffer);
        if (return_value != STATUS_SUCCESS)
            goto exit;

        SDL_Delay(16);
    }

    return_value = STATUS_SUCCESS;

exit:
    cleanup_sdl(window, renderer, texture);
    return return_value;
}
