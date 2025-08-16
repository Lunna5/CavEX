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

// Static data for inventory screen
static struct container_slot inv_slots[INVENTORY_SIZE];
static struct container_screen_data inv_data = {
    .slots = inv_slots,
    .config = {
        .gui_width = 176,
        .gui_height = 167,
        .container_type = WINDOWC_INVENTORY,
        .gui_texture = &texture_gui_inventory,
        .title = "\2478Crafting",
        .title_x = 86 * 2,
        .title_y = 16 * 2,
        .max_slots = INVENTORY_SIZE
    }
};

static void screen_inventory_reset(struct screen* s, int width, int height) {
    input_pointer_enable(true);

    if(gstate.local_player)
        gstate.local_player->data.local_player.capture_input = false;

    s->render3D = screen_ingame.render3D;

    inv_data.pointer_available = false;
    inv_data.pointer_has_item = false;

    // Setup common slots for inventory
    container_setup_common_slots(inv_slots, &inv_data.slots_index,
                                &inv_data.selected_slot, WINDOWC_INVENTORY, INVENTORY_SLOT_MAIN);
}

static void screen_inventory_update(struct screen* s, float dt) {
    // Handle common input
    container_handle_input(&inv_data, WINDOWC_INVENTORY);

    // Find nearest slots for navigation
    container_find_nearest_slots(&inv_data);
}

static void screen_inventory_render2D(struct screen* s, int width, int height) {
    int off_x = (width - inv_data.config.gui_width * 2) / 2;
    int off_y = (height - inv_data.config.gui_height * 2) / 2;

    // Render common UI elements
    container_render_common(&inv_data, width, height);

    // Render player model (inventory specific)
    container_render_player_model(&inv_data, off_x, off_y);
}

struct screen screen_inventory = {
    .reset = screen_inventory_reset,
    .update = screen_inventory_update,
    .render2D = screen_inventory_render2D,
    .render3D = NULL,
    .render_world = true,
};