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

#ifndef CAVEX_CLIENTBOUND_SET_INVENTORY_SLOT_H
#define CAVEX_CLIENTBOUND_SET_INVENTORY_SLOT_H
#include <stdint.h>
#include "../../item/items.h"

typedef struct {
	uint8_t window;
	uint8_t slot;
	struct item_data item;
} clientbound_set_inventory_slot;

#endif // CAVEX_CLIENTBOUND_SET_INVENTORY_SLOT_H
