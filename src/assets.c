#define STB_IMAGE_IMPLEMENTATION
#include "assets.h"
#include "common.h"
#include "stb_image.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void free_image_buffer(uint32_t **image_buffer, int height) {
    if (!image_buffer)
        return;

    for (int y = 0; y < height; y++)
        free(image_buffer[y]);

    free(image_buffer);
}

// Need to free returned image buffer using free_image_buffer method
status_code load_image(const char *file_name, uint32_t ***image_argb_buffer_out, int *width_out, int *height_out) {
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char *data = NULL;
    uint32_t **argb = NULL;
    status_code return_value = STATUS_ERROR;

    data = stbi_load(file_name, &width, &height, &channels, 4);
    if (!data) {
        const char *error_reason = stbi_failure_reason();
        fprintf(stderr, "stbi_load: %s\n", error_reason);
        goto exit;
    }

    argb = malloc(height * sizeof(uint32_t *));
    if (!argb) {
        perror("malloc");
        goto exit;
    }

    for (int y = 0; y < height; y++) {
        argb[y] = malloc(width * sizeof(uint32_t));
        for (int x = 0; x < width; x++) {
            int i = (y * width + x) * 4; // 4 bytes per pixel (RGBA)
            unsigned char r = data[i];
            unsigned char g = data[i + 1];
            unsigned char b = data[i + 2];
            unsigned char a = data[i + 3];
            argb[y][x] = (a << 24) | (r << 16) | (g << 8) | b; // ARGB format
        }
    }

    *image_argb_buffer_out = argb;
    argb = NULL;
    *width_out = width;
    *height_out = height;

    return_value = STATUS_SUCCESS;

exit:
    stbi_image_free(data);
    free_image_buffer(argb, height);

    return return_value;
}
