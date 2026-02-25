#include "hud.h"
#include "assets.h"
#include "common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


static status_code overlay_weapon(uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH], uint32_t **weapon_buffer,
                                  int weapon_height, int weapon_width) {
    int weapon_offset_y = RENDER_HEIGHT - weapon_height; 
    // 100 is a magical number making sure weapon isn't all the way to the left of the screen
    int weapon_offset_x = RENDER_WIDTH - weapon_width - 100;

    for (int y = 0; y < weapon_height; y++) {
        for (int x = 0; x < weapon_width; x++) {
            int frame_y = y + weapon_offset_y;
            int frame_x = x + weapon_offset_x;
            int weapon_pixel =weapon_buffer[y][x];

            // TODO: Properly handle alpha layers that are semi transparent and draw the proper color
            if (!(weapon_pixel & 0xFF000000 )) 
                continue;

            if (frame_y >= RENDER_HEIGHT || frame_x >= RENDER_WIDTH) {
                fprintf(stderr, "Tried to paint weapon outside framebuffer");
                return STATUS_ERROR;
            }

            frame_buffer[frame_y][frame_x] = weapon_pixel;
        }
    }

    return STATUS_SUCCESS;
}

status_code overlay_hud(const player *player, uint32_t frame_buffer[RENDER_HEIGHT][RENDER_WIDTH]) {
    int height = 0;
    int width = 0;
    unsigned int **weapon_buffer = NULL;
    status_code return_value = STATUS_ERROR;

    // TODO: Make this file path not depend on executable being located inside repository.
    // TODO: Check weapon type and load different asset
    // TODO: Handle path being working directory dependent
    return_value = load_image("./assets/shotgun-placeholder.png", &weapon_buffer, &width, &height);
    if (return_value != STATUS_SUCCESS)
        goto exit;

    return_value =overlay_weapon(frame_buffer, weapon_buffer, height, width);
    if (return_value != STATUS_SUCCESS) 
        goto exit;

    return_value = STATUS_SUCCESS;

exit:
    free_image_buffer(weapon_buffer, height);
    return return_value;
}
