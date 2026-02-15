#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <common.h>

status_code render_frame(SDL_Renderer *renderer, SDL_Texture *texture,
                         uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]);
status_code sdl_create_context(char *window_name, SDL_Window **window, SDL_Renderer **renderer,
                                         SDL_Texture **texture);
void cleanup_sdl(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture);

#endif
