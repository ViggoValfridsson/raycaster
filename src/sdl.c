#include "common.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <stdio.h>
#include <stdlib.h>

static void log_sdl_error() {
    const char *error_message = SDL_GetError();
    fprintf(stderr, "%s\n", error_message);
}

status_code render_frame(SDL_Renderer *renderer, SDL_Texture *texture,
                         const uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]) {
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

status_code sdl_create_context(const char *window_name, SDL_Window **window, SDL_Renderer **renderer,
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

static game_event convert_event(SDL_Event *sdl_event) {
    if (sdl_event->type == SDL_QUIT) {
        return EVENT_QUIT;
    } else if (sdl_event->type == SDL_KEYDOWN) {
        switch (sdl_event->key.keysym.sym) {
        case SDLK_ESCAPE:
            return EVENT_QUIT;
        case SDLK_w:
            return EVENT_MOVE_UP;
        case SDLK_s:
            return EVENT_MOVE_DOWN;
        case SDLK_a:
            return EVENT_MOVE_LEFT;
        case SDLK_d:
            return EVENT_MOVE_RIGHT;
        }
    }

    return EVENT_NONE;
}

status_code sdl_get_events(game_event **events_out, int *events_len_out) {
    int event_capacity = 16;
    int events_len = 0;
    status_code return_value = STATUS_SUCCESS;

    game_event *events = malloc(sizeof(game_event) * event_capacity);
    if (!events) {
        perror("malloc");
        goto exit;
    }

    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
        game_event event = convert_event(&sdl_event);

        if (event != EVENT_NONE) {
            if (events_len >= event_capacity) {
                event_capacity *= 2;

                game_event *tmp = realloc(events, sizeof(game_event) * event_capacity);
                if (!tmp) {
                    perror("realloc");
                    goto exit;
                }

                events = tmp;
            }

            events[(events_len++)] = event;
        }
    }

    *events_out = events;
    events = NULL;
    *events_len_out = events_len;

    return_value = STATUS_SUCCESS;

exit:
    free(events);
    return return_value;
}

void cleanup_sdl(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}
