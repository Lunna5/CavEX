/*
	Copyright (c) 2023 ByteBit/xtreme8000

	This file is part of CavEX.

	CavEX is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	CavEX is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with CavEX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../../graphics/gfx_util.h"
#include "../../graphics/gui_util.h"
#include "../../network/server_interface.h"
#include "../../platform/gfx.h"
#include "../../platform/input.h"
#include "../game_state.h"
#include "screen.h"
#include "screen_container_common.h"

// Static data for crafting screen
static struct container_slot craft_slots[CRAFTING_SIZE];
static struct container_screen_data craft_data = {
    .slots = craft_slots,
    .config = {
        .gui_width = 176,
        .gui_height = 167,
        .gui_texture = &texture_gui_crafting,
        .title = "\2478Crafting",
        .title_x = 28 * 2,
        .title_y = 6 * 2,
        .max_slots = CRAFTING_SIZE
    }
};

static uint8_t crafting_container;

void screen_crafting_set_windowc(uint8_t container) {
    crafting_container = container;
    craft_data.config.container_type = container;
}

static void screen_crafting_reset(struct screen* s, int width, int height) {
    input_pointer_enable(true);

    if(gstate.local_player)
        gstate.local_player->data.local_player.capture_input = false;

    s->render3D = screen_ingame.render3D;

    craft_data.pointer_available = false;
    craft_data.pointer_has_item = false;

    // Setup common slots for crafting table
    container_setup_common_slots(craft_slots, &craft_data.slots_index,
                                &craft_data.selected_slot, crafting_container, CRAFTING_SLOT_MAIN);
}

static void screen_crafting_update(struct screen* s, float dt) {
    container_handle_input(&craft_data, crafting_container);

    container_find_nearest_slots(&craft_data);
}

static void screen_crafting_render2D(struct screen* s, int width, int height) {
    container_render_common(&craft_data, width, height);
}

struct screen screen_crafting = {
    .reset = screen_crafting_reset,
    .update = screen_crafting_update,
    .render2D = screen_crafting_render2D,
    .render3D = NULL,
    .render_world = true,
};