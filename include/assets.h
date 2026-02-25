#ifndef ASSETS_H
#define ASSETS_H

#include "common.h"
#include <stdint.h>

void free_image_buffer(uint32_t **image_buffer, int height);
// Need to free returned image buffer using free_image_buffer method
status_code load_image(const char *file_name, uint32_t ***image_argb_buffer_out, int *width_out, int *height_out);

#endif
