/*
Copyright (c) 2025 Lunna5

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

#include "screen_container_common.h"
#include "../../graphics/gfx_util.h"
#include "../../graphics/gui_util.h"
#include "../../graphics/render_model.h"
#include "../../log/log.h"
#include "../../network/server_interface.h"
#include "../../platform/gfx.h"
#include "../../platform/input.h"
#include "../game_state.h"
#include "screen.h"

#include <limits.h>

static void setup_crafting_table_slots(struct container_slot* slots,
									   size_t* slots_index) {
	// Crafting table input slots (3x3)
	for(int k = 0; k < CRAFTING_SIZE_INPUT; k++) {
		slots[(*slots_index)++] = (struct container_slot) {
			.x = (30 + (k % 3) * 18) * 2,
			.y = (17 + (k / 3) * 18) * 2,
			.slot = k + CRAFTING_SLOT_INPUT,
		};
	}

	// Crafting output slot
	slots[(*slots_index)++] = (struct container_slot) {
		.x = 124 * 2,
		.y = 35 * 2,
		.slot = CRAFTING_SLOT_OUTPUT,
	};
}

static void setup_furnace(struct container_slot* slots, size_t* slots_index) {
	// Furnace input slot
	slots[(*slots_index)++] = (struct container_slot) {
		.x = 56 * 2,
		.y = 17 * 2,
		.slot = FURNACE_SLOT_INPUT,
	};

	// Furnace fuel slot
	slots[(*slots_index)++] = (struct container_slot) {
		.x = 56 * 2,
		.y = 53 * 2,
		.slot = FURNACE_SLOT_FUEL,
	};

	// Furnace output slot
	slots[(*slots_index)++] = (struct container_slot) {
		.x = 116 * 2,
		.y = 35 * 2,
		.slot = FURNACE_SLOT_OUTPUT,
	};
}

static void
setup_player_inventory_armor_and_crafting_slots(struct container_slot* slots,
												size_t* slots_index) {
	// Armor slots
	for(int k = 0; k < INVENTORY_SIZE_ARMOR; k++) {
		slots[(*slots_index)++] = (struct container_slot) {
			.x = 8 * 2,
			.y = (8 + k * 18) * 2,
			.slot = k + INVENTORY_SLOT_ARMOR,
		};
	}

	// Personal crafting slots (2x2)
	for(int k = 0; k < INVENTORY_SIZE_CRAFTING; k++) {
		slots[(*slots_index)++] = (struct container_slot) {
			.x = (88 + (k % 2) * 18) * 2,
			.y = (26 + (k / 2) * 18) * 2,
			.slot = k + INVENTORY_SLOT_CRAFTING,
		};
	}

	// Personal crafting output slot
	slots[(*slots_index)++] = (struct container_slot) {
		.x = 144 * 2,
		.y = 36 * 2,
		.slot = INVENTORY_SLOT_OUTPUT,
	};
}

// Setup common slot layouts for inventory and crafting table
void container_setup_common_slots(struct container_slot* slots,
								  size_t* slots_index, size_t* selected_slot,
								  uint8_t container_type,
								  size_t main_inventory_offset) {
	*slots_index = 0;

	// Main inventory slots
	for(int k = 0; k < INVENTORY_SIZE_MAIN; k++) {
		slots[(*slots_index)++] = (struct container_slot) {
			.x = (8 + (k % INVENTORY_SIZE_HOTBAR) * 18) * 2,
			.y = (84 + (k / INVENTORY_SIZE_HOTBAR) * 18) * 2,
			.slot = k + main_inventory_offset, // Offset for main inventory
		};
	}

	// Hotbar slots
	for(int k = 0; k < INVENTORY_SIZE_HOTBAR; k++) {
		if(k
		   == (int)inventory_get_hotbar(
			   windowc_get_latest(gstate.windows[WINDOWC_INVENTORY])))
			*selected_slot = *slots_index;

		slots[(*slots_index)++] = (struct container_slot) {
			.x = (8 + k * 18) * 2,
			.y = (84 + 3 * 18 + 4) * 2,
			.slot = k + main_inventory_offset
				+ INVENTORY_SIZE_MAIN, // Offset for hotbar
		};
	}

	switch(container_type) {
		case WINDOWC_CRAFTING: {
			setup_crafting_table_slots(slots, slots_index);
			break;
		}
		case WINDOWC_INVENTORY: {
			setup_player_inventory_armor_and_crafting_slots(slots, slots_index);
			break;
		}
		case WINDOW_TYPE_FURNACE: {
			setup_furnace(slots, slots_index);
			break;
		}
		default: {
			log_error("Unknown container type: %d", container_type);
		}
	}
}

// Handle input for container screens (inventory, crafting table)
void container_handle_input(struct container_screen_data* data,
							uint8_t container_type) {
	// Handle inventory close
	if(input_pressed(IB_INVENTORY)) {
		svin_rpc_send(&(struct server_rpc) {
			.type = SRPC_WINDOW_CLOSE,
			.payload.window_close.window = container_type,
		});

		screen_set(&screen_ingame);
		return;
	}

	// Handle clicks
	struct inventory* inv = windowc_get_latest(gstate.windows[container_type]);

	if(input_pressed(IB_GUI_CLICK)) {
		uint16_t action_id;
		if(windowc_new_action(gstate.windows[container_type], &action_id, false,
							  data->slots[data->selected_slot].slot)) {
			svin_rpc_send(&(struct server_rpc) {
				.type = SRPC_WINDOW_CLICK,
				.payload.window_click.window = container_type,
				.payload.window_click.action_id = action_id,
				.payload.window_click.right_click = false,
				.payload.window_click.slot
				= data->slots[data->selected_slot].slot,
			});
		}
	} else if(input_pressed(IB_GUI_CLICK_ALT)) {
		uint16_t action_id;
		if(windowc_new_action(gstate.windows[container_type], &action_id, true,
							  data->slots[data->selected_slot].slot)) {
			svin_rpc_send(&(struct server_rpc) {
				.type = SRPC_WINDOW_CLICK,
				.payload.window_click.window = container_type,
				.payload.window_click.action_id = action_id,
				.payload.window_click.right_click = true,
				.payload.window_click.slot
				= data->slots[data->selected_slot].slot,
			});
		}
	}

	// Handle pointer
	data->pointer_available = input_pointer(&data->pointer_x, &data->pointer_y,
											&data->pointer_angle);
}

// Find nearest slots in each direction for keyboard navigation
void container_find_nearest_slots(struct container_screen_data* data) {
	size_t slot_nearest[4] = {data->selected_slot, data->selected_slot,
							  data->selected_slot, data->selected_slot};
	int slot_dist[4] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX};
	int pointer_slot = -1;

	int off_x = (gfx_width() - data->config.gui_width * 2) / 2;
	int off_y = (gfx_height() - data->config.gui_height * 2) / 2;

	// Find nearest slots in each direction and detect pointer hover
	for(size_t k = 0; k < data->slots_index; k++) {
		int dx = data->slots[k].x - data->slots[data->selected_slot].x;
		int dy = data->slots[k].y - data->slots[data->selected_slot].y;

		// Check if pointer is over this slot
		if(data->pointer_available
		   && data->pointer_x >= off_x + data->slots[k].x
		   && data->pointer_x < off_x + data->slots[k].x + 16 * 2
		   && data->pointer_y >= off_y + data->slots[k].y
		   && data->pointer_y < off_y + data->slots[k].y + 16 * 2) {
			pointer_slot = k;
		}

		// Calculate distances for directional navigation
		int distx = dx * dx + dy * dy * 8;
		int disty = dx * dx * 8 + dy * dy;

		// Left
		if(dx < 0 && distx < slot_dist[0]) {
			slot_nearest[0] = k;
			slot_dist[0] = distx;
		}
		// Right
		if(dx > 0 && distx < slot_dist[1]) {
			slot_nearest[1] = k;
			slot_dist[1] = distx;
		}
		// Up
		if(dy < 0 && disty < slot_dist[2]) {
			slot_nearest[2] = k;
			slot_dist[2] = disty;
		}
		// Down
		if(dy > 0 && disty < slot_dist[3]) {
			slot_nearest[3] = k;
			slot_dist[3] = disty;
		}
	}

	// Update selected slot based on input
	if(data->pointer_available && pointer_slot >= 0) {
		data->selected_slot = pointer_slot;
		data->pointer_has_item = true;
	} else {
		if(input_pressed(IB_GUI_LEFT)) {
			data->selected_slot = slot_nearest[0];
			data->pointer_has_item = false;
		}
		if(input_pressed(IB_GUI_RIGHT)) {
			data->selected_slot = slot_nearest[1];
			data->pointer_has_item = false;
		}
		if(input_pressed(IB_GUI_UP)) {
			data->selected_slot = slot_nearest[2];
			data->pointer_has_item = false;
		}
		if(input_pressed(IB_GUI_DOWN)) {
			data->selected_slot = slot_nearest[3];
			data->pointer_has_item = false;
		}
	}
}

// Render the player model with equipped items (only for inventory)
void container_render_player_model(struct container_screen_data* data,
								   int off_x, int off_y) {
	struct inventory* inv
		= windowc_get_latest(gstate.windows[WINDOWC_INVENTORY]);
	struct container_slot* selection = &data->slots[data->selected_slot];

	// Calculate angle for player model based on cursor position
	float angle_x
		= atan2f((data->pointer_has_item ? data->pointer_x :
										   off_x + selection->x + 8 * 2)
					 - (off_x + 52 * 2),
				 192.0F);
	float angle_y
		= atan2f((data->pointer_has_item ? data->pointer_y :
										   off_y + selection->y + 8 * 2)
					 - (off_y + 19 * 2),
				 192.0F);

	// Setup view matrix for player model
	mat4 view;
	glm_mat4_identity(view);
	glm_translate(view, (vec3) {off_x + 52 * 2, off_y + 39 * 2, 0.0F});
	glm_scale(view, (vec3) {3.75F, -3.75F, 1.0F});
	glm_rotate_x(view, angle_y * 0.66F * 0.5F, view);
	glm_rotate_y(view, angle_x * 0.5F, view);

	// Draw player model
	gfx_write_buffers(true, true, true);
	struct item_data held_item, helmet, chestplate, leggings, boots;
	render_model_player(
		view, glm_deg(angle_y * 0.66F * 0.5F), glm_deg(angle_x * 0.5F), 0.0F,
		0.0F, inventory_get_hotbar_item(inv, &held_item) ? &held_item : NULL,
		inventory_get_slot(inv, INVENTORY_SLOT_ARMOR + 0, &helmet) ? &helmet :
																	 NULL,
		inventory_get_slot(inv, INVENTORY_SLOT_ARMOR + 1, &chestplate) ?
			&chestplate :
			NULL,
		inventory_get_slot(inv, INVENTORY_SLOT_ARMOR + 2, &leggings) ?
			&leggings :
			NULL,
		inventory_get_slot(inv, INVENTORY_SLOT_ARMOR + 3, &boots) ? &boots :
																	NULL);
	gfx_write_buffers(true, false, false);
	gfx_matrix_modelview(GLM_MAT4_IDENTITY);
}

// Common rendering code for container screens
void container_render_common(struct container_screen_data* data, int width,
							 int height) {
	struct inventory* inv
		= windowc_get_latest(gstate.windows[data->config.container_type]);

	// Darken background
	gfx_texture(false);
	gutil_texquad_col(0, 0, 0, 0, 0, 0, width, height, 0, 0, 0, 180);
	gfx_texture(true);

	int off_x = (width - data->config.gui_width * 2) / 2;
	int off_y = (height - data->config.gui_height * 2) / 2;

	// Draw container GUI
	gfx_bind_texture(data->config.gui_texture);
	gutil_texquad(off_x, off_y, 0, 0, data->config.gui_width,
				  data->config.gui_height, data->config.gui_width * 2,
				  data->config.gui_height * 2);

	// Draw title
	if(data->config.title) {
		gutil_text(off_x + data->config.title_x, off_y + data->config.title_y,
				   data->config.title, 16, false);
	}

	struct container_slot* selection = &data->slots[data->selected_slot];
	// Draw items in slots
	for(size_t k = 0; k < data->slots_index; k++) {
		struct item_data item;
		if((data->selected_slot != k || !inventory_get_picked_item(inv, NULL)
			|| (data->pointer_available && data->pointer_has_item))
		   && inventory_get_slot(inv, data->slots[k].slot, &item)) {
			gutil_draw_item(&item, off_x + data->slots[k].x,
							off_y + data->slots[k].y, 1);
		}
	}

	// Draw selection highlight
	gfx_bind_texture(&texture_gui2);
	gutil_texquad(off_x + selection->x - 8, off_y + selection->y - 8, 208, 0,
				  24, 24, 24 * 2, 24 * 2);

	// Draw control icons
	int icon_offset = 32;
	icon_offset += gutil_control_icon(icon_offset, IB_GUI_UP, "Move");

	if(inventory_get_picked_item(inv, NULL)) {
		icon_offset
			+= gutil_control_icon(icon_offset, IB_GUI_CLICK, "Swap item");
		icon_offset
			+= gutil_control_icon(icon_offset, IB_GUI_CLICK_ALT, "Place one");
	} else if(inventory_get_slot(inv, selection->slot, NULL)) {
		icon_offset
			+= gutil_control_icon(icon_offset, IB_GUI_CLICK, "Pickup item");
		icon_offset
			+= gutil_control_icon(icon_offset, IB_GUI_CLICK_ALT, "Split stack");
	}

	icon_offset += gutil_control_icon(icon_offset, IB_INVENTORY, "Leave");

	// Draw held/hovered item
	struct item_data item;
	if(inventory_get_picked_item(inv, &item)) {
		if(data->pointer_available && data->pointer_has_item) {
			gutil_draw_item(&item, data->pointer_x - 8 * 2,
							data->pointer_y - 8 * 2, 0);
		} else {
			gutil_draw_item(&item, off_x + selection->x, off_y + selection->y,
							0);
		}
	} else if(inventory_get_slot(inv, selection->slot, &item)) {
		// Show item name tooltip
		char* tmp = item_get(&item) ? item_get(&item)->name : "Unknown";
		gfx_blending(MODE_BLEND);
		gfx_texture(false);
		gutil_texquad_col(off_x + selection->x - 4 + 16
							  - gutil_font_width(tmp, 16) / 2,
						  off_y + selection->y - 4 + 46, 0, 0, 0, 0,
						  gutil_font_width(tmp, 16) + 7, 16 + 8, 0, 0, 0, 180);
		gfx_texture(true);
		gfx_blending(MODE_OFF);

		gutil_text(off_x + selection->x + 16 - gutil_font_width(tmp, 16) / 2,
				   off_y + selection->y + 46, tmp, 16, false);
	}

	// Draw pointer
	if(data->pointer_available) {
		gfx_bind_texture(inventory_get_picked_item(inv, NULL) ?
							 &texture_pointer_grab :
							 &texture_pointer);
		gutil_texquad_rt_any(data->pointer_x, data->pointer_y,
							 glm_rad(data->pointer_angle), 0, 0, 256, 256, 96,
							 96);
	}
}
