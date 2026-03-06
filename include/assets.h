#ifndef ASSETS_H
#define ASSETS_H

#include "common.h"
#include "game.h"
#include <stdint.h>

void destroy_assets();
// After calling init assets you must call destroy assets to free memory. Do not call this function multiple times
// without destroying assets inbetween  as that will leak memory.
status_code init_assets();
// Do not call this before init_assets
// Never free sprite with call to libc free()
status_code get_weapon_sprite(weapon_type type, sprite **sprite_out);
// Do not call this before init_assets
// Never free sprite with call to libc free()
status_code get_enemy_sprite(enemy_type type, enemy_state state, int state_frame, sprite **sprite_out);

#endif
