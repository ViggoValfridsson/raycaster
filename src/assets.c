#define STB_IMAGE_IMPLEMENTATION
#include "assets.h"
#include "common.h"
#include "stb_image.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void free_sprite(sprite *sprite) {
    if (!sprite)
        return;

    if (sprite->pixels)
        free(sprite->pixels);

    if (sprite)
        free(sprite);
}

// Need to free returned image buffer using free_image_buffer method
status_code load_sprite(const char *file_name, sprite **sprite_out) {
    unsigned char *data = NULL;
    sprite *image = NULL;
    status_code return_value = STATUS_ERROR;

    image = malloc(sizeof(sprite));
    if (!image) {
        perror("malloc");
        goto exit;
    }

    int channels = 0;
    data = stbi_load(file_name, &image->width, &image->height, &channels, 4);
    if (!data) {
        const char *error_reason = stbi_failure_reason();
        fprintf(stderr, "stbi_load: %s\n", error_reason);
        goto exit;
    }

    image->pixels = malloc(image->height * image->width * sizeof(uint32_t));
    if (!image->pixels) {
        perror("malloc");
        goto exit;
    }

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            int i = (y * image->width + x) * 4; // 4 bytes per pixel (RGBA)
            unsigned char r = data[i];
            unsigned char g = data[i + 1];
            unsigned char b = data[i + 2];
            unsigned char a = data[i + 3];
            image->pixels[y * image->width + x] = (a << 24) | (r << 16) | (g << 8) | b; // ARGB format
        }
    }

    *sprite_out = image;
    image = NULL;

    return_value = STATUS_SUCCESS;

exit:
    stbi_image_free(data);
    free_sprite(image);

    return return_value;
}
