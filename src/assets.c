#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "assets.h"
#include "common.h"
#include "game.h"
#include "stb_image.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE_NAME_LEN 128

void free_sprite(sprite *sprite) {
    if (!sprite)
        return;

    if (sprite->pixels)
        free(sprite->pixels);

    if (sprite)
        free(sprite);
}

status_code get_weapon_sprite_name(weapon_type type, char *buffer, size_t len) {
    const char *file;

    switch (type) {
    case shotgun:
        file = "./assets/shotgun-placeholder.png";
        break;
    default:
        fprintf(stderr, "No sprite for weapon type %d\n", type);
        return STATUS_ERROR;
    }

    size_t file_len = strlen(file) + 1;
    if (file_len > len) {
        fprintf(stderr, "Buffer too small, required size for weapon type %d is %zu\n", type, file_len);
        return STATUS_ERROR;
    }

    memcpy(buffer, file, file_len);

    return STATUS_SUCCESS;
}

// Need to free returned image buffer using free_sprite method
status_code get_weapon_sprite(weapon_type type, sprite **sprite_out) {
    unsigned char *data = NULL;
    sprite *image = NULL;
    status_code return_value = STATUS_ERROR;

    char file_name[MAX_FILE_NAME_LEN];
    return_value = get_weapon_sprite_name(type, file_name, MAX_FILE_NAME_LEN);
    if (return_value != STATUS_SUCCESS)
        goto exit;

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
            int index = y * image->width + x;
            int i = index * 4; // 4 bytes per pixel (RGBA)
            unsigned char r = data[i];
            unsigned char g = data[i + 1];
            unsigned char b = data[i + 2];
            unsigned char a = data[i + 3];
            image->pixels[index] = (a << 24) | (r << 16) | (g << 8) | b; // ARGB format
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
