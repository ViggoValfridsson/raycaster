#ifndef ASSETS_H
#define ASSETS_H

#include "common.h"
#include <stdint.h>

void free_sprite(sprite *sprite);
// Need to free returned image buffer using free_image_buffer method
status_code load_sprite(const char *file_name, sprite **sprite_out);

#endif
