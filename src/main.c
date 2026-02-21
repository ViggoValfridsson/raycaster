#include "common.h"
#include "game.h"
#include "raycast.h"
#include "sdl.h"
#include <SDL2/SDL_timer.h>

#define FRAME_LENGTH_MS 16

int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    game_event *game_events = NULL;
    status_code return_value = STATUS_ERROR;

    return_value = sdl_create_context("Raycaster", &window, &renderer, &texture);
    if (return_value != STATUS_SUCCESS)
        goto exit;

    game_state state;
    init_game(&state);

    uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH];

    while (state.is_running) {
        uint64_t start_ticks = SDL_GetTicks64();
        int events_len;
        return_value = sdl_get_events(&game_events, &events_len);
        if (return_value != STATUS_SUCCESS)
            goto exit;

        handle_events(&state, game_events, events_len);
        raycast(&state, frame_buffer);

        return_value = render_frame(renderer, texture, frame_buffer);
        if (return_value != STATUS_SUCCESS)
            goto exit;

        free(game_events);
        game_events = NULL;

        uint64_t render_time_ms = SDL_GetTicks64() - start_ticks;
        SDL_Delay(render_time_ms < FRAME_LENGTH_MS ? FRAME_LENGTH_MS - render_time_ms : 0);
    }

    return_value = STATUS_SUCCESS;

exit:
    free(game_events);
    cleanup_sdl(window, renderer, texture);

    return return_value;
}
