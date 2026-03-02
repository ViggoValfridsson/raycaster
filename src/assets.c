#define STB_IMAGE_IMPLEMENTATION
#include "assets.h"
#include "common.h"
#include "game.h"
#include "stb_image.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE_NAME_LEN 128
#define WEAPON_ASSET_COUNT (sizeof(weapon_assets) / sizeof(weapon_assets[0]))

typedef struct {
    weapon_type type;
    sprite *sprite;
} weapon_cache_entry;

typedef struct {
    weapon_type type;
    const char *file;
} weapon_asset_desc;

static weapon_cache_entry *weapon_cache;
static int weapon_cache_len;
static const weapon_asset_desc weapon_assets[] = {{shotgun, "./assets/shotgun-placeholder.png"}};

static void free_sprite(sprite *sprite) {
    if (!sprite)
        return;

    free(sprite->pixels);
    free(sprite);
}

static void free_weapon_cache_entry(weapon_cache_entry *entry) {
    if (!entry)
        return;

    free_sprite(entry->sprite);
}

static void free_weapon_cache() {
    if (!weapon_cache)
        return;

    for (int i = 0; i < weapon_cache_len; i++)
        free_weapon_cache_entry(&weapon_cache[i]);

    free(weapon_cache);
}

void destroy_assets() {
    free_weapon_cache();

    weapon_cache = NULL;
    weapon_cache_len = 0;
}

// TODO: extract RGBA conversion to separate function
// TODO: Fix sprite loading being dependent on where game is run from
static status_code load_sprite(const char *file_name, sprite **sprite_out) {
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

static status_code init_weapon_assets() {
    weapon_cache = calloc(WEAPON_ASSET_COUNT, sizeof(weapon_cache_entry));
    if (!weapon_cache) {
        perror("calloc");
        return STATUS_ERROR;
    }

    for (size_t i = 0; i < WEAPON_ASSET_COUNT; i++) {
        weapon_cache[i].type = weapon_assets[i].type;

        status_code return_value = load_sprite(weapon_assets[i].file, &weapon_cache[i].sprite);
        if (return_value != STATUS_SUCCESS) {
            weapon_cache_len = i;
            return return_value;
        }

        weapon_cache_len = i + 1;
    }

    return STATUS_SUCCESS;
}

// After calling init assets you must call destroy assets to free memory. Do not call this function multiple times
// without destroying assets as that will leak memory.
status_code init_assets() {
    status_code return_value = init_weapon_assets();
    if (return_value)
        goto cleanup;

    return STATUS_SUCCESS;

cleanup:
    destroy_assets();
    return return_value;
}

// Do not call this before init_assets
// Never free sprite with call to libc free()
status_code get_weapon_sprite(weapon_type type, sprite **sprite_out) {
    for (int i = 0; i < weapon_cache_len; i++) {
        if (weapon_cache[i].type == type) {
            *sprite_out = weapon_cache[i].sprite;
            return STATUS_SUCCESS;
        }
    }

    fprintf(stderr, "Weapon type did not exist in cache memory\n");
    return STATUS_ERROR;
}
