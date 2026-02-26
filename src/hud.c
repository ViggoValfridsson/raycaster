#include "hud.h"
#include "assets.h"
#include "common.h"
#include "game.h"
#include <stdint.h>
#include <stdlib.h>

static status_code overlay_weapon(uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH], sprite *sprite, const weapon*weapon) {
    int weapon_offset_y = RENDER_HEIGHT - sprite->height;
    int weapon_offset_x = RENDER_WIDTH - sprite->width - weapon->x_margin;

    for (int y = 0; y < sprite->height; y++) {
        for (int x = 0; x < sprite->width; x++) {
            int frame_y = y + weapon_offset_y;
            int frame_x = x + weapon_offset_x;
            int weapon_pixel = sprite->pixels[y * sprite->width + x];

            // This doesn't respect semi transparency. Since we never use semi transparency this is a feature not a bug
            if (!(weapon_pixel & 0xFF000000))
                continue;

            frame_buffer[frame_y][frame_x] = weapon_pixel;
        }
    }

    return STATUS_SUCCESS;
}

status_code overlay_hud(const player *player, uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]) {
    sprite *sprite = NULL;
    status_code return_value = STATUS_ERROR;

    // TODO: Make this file path not depend on executable being located inside repository.
    // TODO: Check weapon type and load different asset
    // TODO: Handle path being working directory dependent
    return_value = get_weapon_sprite(player->weapon.type, &sprite);
    if (return_value != STATUS_SUCCESS)
        goto exit;

    return_value = overlay_weapon(frame_buffer, sprite, &player->weapon);
    if (return_value != STATUS_SUCCESS)
        goto exit;

    return_value = STATUS_SUCCESS;

exit:
    free_sprite(sprite);
    return return_value;
}
