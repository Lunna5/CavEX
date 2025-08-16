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

#ifndef CAVEX_SCREEN_CONTAINER_COMMON_H
#define CAVEX_SCREEN_CONTAINER_COMMON_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../game_state.h"
#include "screen.h"
#include <stdbool.h>
#include <stddef.h>

// Structure for representing a slot in any container GUI
struct container_slot {
	int x, y;
	size_t slot;
};

// Configuration for container screens
struct container_config {
	// GUI properties
	int gui_width;
	int gui_height;

	// Container type
	uint8_t container_type;

	// GUI texture
	struct tex_gfx* gui_texture;

	// Title text
	const char* title;
	int title_x;
	int title_y;

	// Maximum number of slots
	size_t max_slots;
};

// Common data structure for container screens
struct container_screen_data {
	bool pointer_has_item;
	bool pointer_available;
	float pointer_x, pointer_y, pointer_angle;
	struct container_slot* slots;
	size_t slots_index;
	size_t selected_slot;
	struct container_config config;
};

// Functions to share code between container screens
void container_setup_common_slots(struct container_slot* slots,
								  size_t* slots_index, size_t* selected_slot,
								  uint8_t container_type,
								  size_t main_inventory_offset);

void container_handle_input(struct container_screen_data* data,
							uint8_t container_type);
void container_find_nearest_slots(struct container_screen_data* data);
void container_render_common(struct container_screen_data* data, int width,
							 int height);
void container_render_player_model(struct container_screen_data* data,
								   int off_x, int off_y);

#ifdef __cplusplus
}
#endif
#endif // CAVEX_SCREEN_CONTAINER_COMMON_H
