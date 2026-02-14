#include "common.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>

#define RENDER_WIDTH 128
#define RENDER_HEIGHT 128
#define RENDER_SIZE RENDER_WIDTH *RENDER_HEIGHT
#define WINDOW_SCALE 8
#define WINDOW_HEIGHT RENDER_HEIGHT *WINDOW_SCALE
#define WINDOW_WIDTH RENDER_WIDTH *WINDOW_SCALE

// TODO: Move this to separate source file later
// TODO actually check and return correct status
status_code render_frame(SDL_Renderer *renderer, SDL_Texture *texture,
                         uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]) {
    SDL_UpdateTexture(texture, NULL, frame_buffer, RENDER_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    return STATUS_SUCCESS;
}

void log_sdl_error() {
    const char *error_message = SDL_GetError();
    fprintf(stderr, "%s", error_message);
}

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

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    int return_value = STATUS_ERROR;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_sdl_error();
        goto exit;
    }

    if (!(window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                    WINDOW_HEIGHT, SDL_WINDOW_SHOWN))) {
        log_sdl_error();
        goto exit;
    }

    if (!(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))) {
        log_sdl_error();
        goto exit;
    }

    if (!(texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH,
                                      RENDER_HEIGHT))) {
        log_sdl_error();
        goto exit;
    }

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
        render_frame(renderer, texture, frame_buffer);

        // TODO: lower this number
        SDL_Delay(200);
    }

exit:
    if (texture)
        SDL_DestroyTexture(texture);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);

    SDL_Quit();

    return return_value;
}
