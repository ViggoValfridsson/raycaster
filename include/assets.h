#ifndef ASSETS_H
#define ASSETS_H

#include "common.h"
#include "game.h"
#include <stdint.h>

void free_sprite(sprite *sprite);
// Need to free returned image buffer using free_image_buffer method
status_code get_weapon_sprite(weapon_type type, sprite **sprite_out);

#endif
