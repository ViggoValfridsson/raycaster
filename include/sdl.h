#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <common.h>
#include "game.h"

#define ARGB_RED 0xFFFF0000
#define ARGB_GREEN 0xFF00FF00
#define ARGB_BLUE 0xFF0000FF
#define ARGB_YELLOW 0XFFFFFF00
#define ARGB_GRAY 0XFF808080
#define ARGB_SKY_BLUE 0XFF87CEFA
#define ARGB_BLACK 0X00000000

status_code render_frame(SDL_Renderer *renderer, SDL_Texture *texture,
                         uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]);
status_code sdl_create_context(char *window_name, SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture);
void cleanup_sdl(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture);
status_code sdl_get_events(game_event **events_out, int *events_len_out);

#endif
