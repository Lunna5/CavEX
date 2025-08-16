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

static struct container_slot furnace_slots[FURNACE_SIZE];
static struct container_screen_data furnace_data
	= {.slots = furnace_slots,
	   .config = {.gui_width = 176,
				  .gui_height = 167,
				  .gui_texture = &texture_gui_furnace,
				  .title = "\2478Furnace",
				  .title_x = 60 * 2,
				  .title_y = 6 * 2,
				  .max_slots = FURNACE_SIZE}};

static uint8_t furnace_container;

void screen_furnace_set_windowc(uint8_t container) {
	furnace_container = container;
	furnace_data.config.container_type = container;
}

static void screen_furnace_reset(struct screen* s, int width, int height) {
	input_pointer_enable(true);

	if(gstate.local_player)
		gstate.local_player->data.local_player.capture_input = false;
	s->render3D = screen_ingame.render3D;

	furnace_data.pointer_available = false;
	furnace_data.pointer_has_item = false;

	container_setup_common_slots(furnace_slots, &furnace_data.slots_index,
								 &furnace_data.selected_slot, furnace_container,
								 FURNACE_SLOT_MAIN);
}

static void screen_furnace_update(struct screen* s, float dt) {
	container_handle_input(&furnace_data, furnace_container);

	container_find_nearest_slots(&furnace_data);
}

static void screen_furnace_render2D(struct screen* s, int width, int height) {
	container_render_common(&furnace_data, width, height);
}

struct screen screen_furnace = {
	.reset = screen_furnace_reset,
	.update = screen_furnace_update,
	.render2D = screen_furnace_render2D,
	.render3D = NULL,
	.render_world = true,
};
