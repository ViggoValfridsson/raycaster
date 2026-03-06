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
#define ENEMY_ASSET_COUNT (sizeof(enemy_assets) / sizeof(enemy_assets[0]))

typedef struct {
    weapon_type type;
    sprite *sprite;
} weapon_cache_entry;

typedef struct {
    weapon_type type;
    const char *file;
} weapon_asset_desc;

typedef struct {
    enemy_type type;
    enemy_state state;
    int state_frame;
    sprite *sprite;
} enemy_cache_entry;

typedef struct {
    enemy_type type;
    enemy_state state;
    int state_frame;
    const char *file;
} enemy_asset_desc;

static weapon_cache_entry *weapon_cache;
static int weapon_cache_len;

static enemy_cache_entry *enemy_cache;
static int enemy_cache_len;

static const weapon_asset_desc weapon_assets[] = {{shotgun, "./assets/shotgun-placeholder.png"}};
static const enemy_asset_desc enemy_assets[] = {{demon, idle, 1, "./assets/enemies/demon/idle-1.png"},
                                                {demon, idle, 2, "./assets/enemies/demon/idle-2.png"},
                                                {demon, idle, 3, "./assets/enemies/demon/idle-3.png"},
                                                {demon, idle, 4, "./assets/enemies/demon/idle-4.png"}};

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

static void free_enemy_weapon_cache_entry(enemy_cache_entry *entry) {
    if (!entry)
        return;

    free_sprite(entry->sprite);
}

static void free_enemy_cache() {
    if (!enemy_cache)
        return;

    for (int i = 0; i < enemy_cache_len; i++)
        free_enemy_weapon_cache_entry(&enemy_cache[i]);

    free(enemy_cache);
}

void destroy_assets() {
    free_weapon_cache();
    free_enemy_cache();

    weapon_cache = NULL;
    weapon_cache_len = 0;

    enemy_cache = NULL;
    enemy_cache_len = 0;
}

static void sprite_fill_argb(sprite *target, const unsigned char *rgba_data) {
    for (int y = 0; y < target->height; y++) {
        for (int x = 0; x < target->width; x++) {
            int index = y * target->width + x;
            int i = index * 4; // 4 bytes per pixel RGBA
            unsigned char r = rgba_data[i];
            unsigned char g = rgba_data[i + 1];
            unsigned char b = rgba_data[i + 2];
            unsigned char a = rgba_data[i + 3];
            target->pixels[index] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }
}

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

    sprite_fill_argb(image, data);

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

static status_code init_enemy_assets() {
    enemy_cache = calloc(ENEMY_ASSET_COUNT, sizeof(enemy_cache_entry));
    if (!enemy_cache) {
        perror("calloc");
        return STATUS_ERROR;
    }

    for (size_t i = 0; i < ENEMY_ASSET_COUNT; i++) {
        enemy_cache[i].type = enemy_assets[i].type;
        enemy_cache[i].state = enemy_assets[i].state;
        enemy_cache[i].state_frame = enemy_assets[i].state_frame;

        status_code return_value = load_sprite(enemy_assets[i].file, &enemy_cache[i].sprite);
        if (return_value != STATUS_SUCCESS) {
            enemy_cache_len = i;
            return return_value;
        }

        enemy_cache_len = i + 1;
    }

    return STATUS_SUCCESS;
}

// After calling init assets you must call destroy assets to free memory. Do not call this function multiple times
// without destroying assets as that will leak memory.
status_code init_assets() {
    status_code return_value = init_weapon_assets();
    if (return_value != STATUS_SUCCESS)
        goto cleanup;

    return_value = init_enemy_assets();
    if (return_value != STATUS_SUCCESS)
        goto cleanup;

    return return_value;

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

    fprintf(stderr, "Weapon type \"%d\" did not exist in cache. Have you called init_assets?\n", type);
    return STATUS_ERROR;
}

// Do not call this before init_assets
// Never free sprite with call to libc free()
status_code get_enemy_sprite(enemy_type type, enemy_state state, int state_frame, sprite **sprite_out) {
    for (int i = 0; i < enemy_cache_len; i++) {
        if (enemy_cache[i].type == type && enemy_cache[i].state == state && enemy_cache[i].state_frame == state_frame) {
            *sprite_out = enemy_cache[i].sprite;
            return STATUS_SUCCESS;
        }
    }

    fprintf(stderr, "Enemy sprite did not exist. Type: %d. State: %d. Frame: %d", type, state, state_frame);
    return STATUS_ERROR;
}
