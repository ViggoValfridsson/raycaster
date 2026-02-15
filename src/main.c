#include "common.h"
#include "sdl.h"

// TODO: remove this temp method
void temp_create_checkerboard(uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]) {
    static int changing = 0;
    changing = changing == 0 ? 1 : 0;

    int checker_size = changing == 1 ? 16 : 8;

    for (int y = 0; y < RENDER_HEIGHT; y++) {
        for (int x = 0; x < RENDER_WIDTH; x++) {
            int checker = ((x / checker_size) + (y / checker_size)) % 2;
            frame_buffer[y][x] = checker ? 0xFFFF0000 : 0xFF0000FF;
        }
    }
}

int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    status_code return_value = STATUS_ERROR;

    return_value = sdl_create_context("Raycaster", &window, &renderer, &texture);
    if (return_value != STATUS_SUCCESS)
        goto exit;

    uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH];
    SDL_Event event;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
        }

        temp_create_checkerboard(frame_buffer);

        return_value = render_frame(renderer, texture, frame_buffer);
        if (return_value != STATUS_SUCCESS)
            goto exit;

        // INFO: Temp high number to make testing easier
        SDL_Delay(300);
    }

    return_value = STATUS_SUCCESS;

exit:
    cleanup_sdl(window, renderer, texture);
    return return_value;
}
