#include "common.h"
#include <SDL2/SDL.h>

static void log_sdl_error() {
    const char *error_message = SDL_GetError();
    fprintf(stderr, "%s\n", error_message);
}

status_code render_frame(SDL_Renderer *renderer, SDL_Texture *texture,
                         uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]) {
    status_code return_value = STATUS_RENDER_ERROR;

    if (SDL_UpdateTexture(texture, NULL, frame_buffer, RENDER_WIDTH * sizeof(uint32_t)) != 0) {
        log_sdl_error();
        goto exit;
    }

    if (SDL_RenderClear(renderer) != 0) {
        log_sdl_error();
        goto exit;
    }

    if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0) {
        log_sdl_error();
        goto exit;
    }

    SDL_RenderPresent(renderer);

    return_value = STATUS_SUCCESS;

exit:
    return return_value;
}

status_code sdl_create_context(char *window_name, SDL_Window **window, SDL_Renderer **renderer,
                                         SDL_Texture **texture) {
    status_code return_value = STATUS_ERROR;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_sdl_error();
        goto exit;
    }

    if (!(*window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                    WINDOW_HEIGHT, SDL_WINDOW_SHOWN))) {
        log_sdl_error();
        goto exit;
    }

    if (!(*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED))) {
        log_sdl_error();
        goto exit;
    }

    if (!(*texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH,
                                      RENDER_HEIGHT))) {
        log_sdl_error();
        goto exit;
    }

    return_value = STATUS_SUCCESS;

exit:
    return return_value;
}

void cleanup_sdl(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}
