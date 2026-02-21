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

status_code sdl_create_context(const char *window_name, SDL_Window **window_out, SDL_Renderer **renderer_out,
                               SDL_Texture **texture_out) {
    status_code return_value = STATUS_ERROR;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_sdl_error();
        goto exit;
    }

    if (!(*window_out = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                         WINDOW_HEIGHT, SDL_WINDOW_SHOWN))) {
        log_sdl_error();
        goto exit;
    }

    if (!(*renderer_out = SDL_CreateRenderer(*window_out, -1, SDL_RENDERER_ACCELERATED))) {
        log_sdl_error();
        goto exit;
    }

    if (!(*texture_out = SDL_CreateTexture(*renderer_out, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                           RENDER_WIDTH, RENDER_HEIGHT))) {
        log_sdl_error();
        goto exit;
    }

    return_value = STATUS_SUCCESS;

exit:
    return return_value;
}

static status_code add_event(game_event **game_events_out, int *events_len, int *event_capacity, game_event event) {
    if (event != EVENT_NONE) {
        if (*events_len >= *event_capacity) {
            *event_capacity *= 2;

            game_event *tmp = realloc(*game_events_out, sizeof(game_event) * *event_capacity);
            if (!tmp) {
                perror("realloc");
                return STATUS_ERROR;
            }

            *game_events_out = tmp;
        }

        (*game_events_out)[((*events_len)++)] = event;
    }

    return STATUS_SUCCESS;
}

static status_code add_held_key_events(game_event **game_events_out, int *events_len, int *event_capacity,
                                       const Uint8 *keys) {
    if (keys[SDL_SCANCODE_W] &&
        (add_event(game_events_out, events_len, event_capacity, EVENT_MOVE_UP)) != STATUS_SUCCESS)
        return STATUS_ERROR;

    if (keys[SDL_SCANCODE_S] &&
        (add_event(game_events_out, events_len, event_capacity, EVENT_MOVE_DOWN)) != STATUS_SUCCESS)
        return STATUS_ERROR;

    if (keys[SDL_SCANCODE_D] &&
        (add_event(game_events_out, events_len, event_capacity, EVENT_MOVE_RIGHT)) != STATUS_SUCCESS)
        return STATUS_ERROR;

    if (keys[SDL_SCANCODE_A] &&
        (add_event(game_events_out, events_len, event_capacity, EVENT_MOVE_LEFT)) != STATUS_SUCCESS)
        return STATUS_ERROR;

    return STATUS_SUCCESS;
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
        if (sdl_event.type != SDL_QUIT && !(sdl_event.type == SDL_KEYDOWN && sdl_event.key.keysym.sym == SDLK_ESCAPE)) {
            continue;
        }

        if ((return_value = add_event(&events, &events_len, &event_capacity, EVENT_QUIT)) != STATUS_SUCCESS)
            goto exit;
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if ((return_value = add_held_key_events(&events, &events_len, &event_capacity, keys)) != STATUS_SUCCESS)
        goto exit;

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
