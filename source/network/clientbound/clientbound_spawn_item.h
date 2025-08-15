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

#ifndef CAVEX_CLIENTBOUND_SPAWN_ITEM_H
#define CAVEX_CLIENTBOUND_SPAWN_ITEM_H
#include <stdint.h>
#include "../../cglm/cglm.h"
#include "../../item/items.h"

typedef struct {
	uint32_t entity_id;
	struct item_data item;
	vec3 pos;
} clientbound_spawn_item;

#endif // CAVEX_CLIENTBOUND_SPAWN_ITEM_H
