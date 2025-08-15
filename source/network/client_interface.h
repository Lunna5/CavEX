/*
	Copyright (c) 2022 ByteBit/xtreme8000

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

#ifndef CLIENT_INTERFACE_H
#define CLIENT_INTERFACE_H

#include "clientbound/clientbound_entity_destroy.h"
#include "clientbound/clientbound_entity_move.h"
#include "clientbound/clientbound_load_chunk.h"
#include "clientbound/clientbound_pickup_item.h"
#include "clientbound/clientbound_player_pos.h"
#include "clientbound/clientbound_set_inventory_slot.h"
#include "clientbound/clientbound_set_time.h"
#include "clientbound/clientbound_spawn_item.h"
#include "clientbound/clientbound_unload_chunk.h"
#include "clientbound/clientbound_update_block.h"
#include "clientbound/clientbound_window_open.h"
#include "clientbound/clientbound_window_transaction.h"
#include "clientbound/clientbound_world_reset.h"

enum client_rpc_type {
	CRPC_CHUNK,
	CRPC_UNLOAD_CHUNK,
	CRPC_INVENTORY_SLOT,
	CRPC_PLAYER_POS,
	CRPC_TIME_SET,
	CRPC_WORLD_RESET,
	CRPC_SET_BLOCK,
	CRPC_WINDOW_TRANSACTION,
	CRPC_SPAWN_ITEM,
	CRPC_PICKUP_ITEM,
	CRPC_ENTITY_DESTROY,
	CRPC_ENTITY_MOVE,
	CRPC_OPEN_WINDOW,
};

typedef struct {
	enum client_rpc_type type;
	union {
		clientbound_load_chunk chunk;
		clientbound_unload_chunk unload_chunk;
		clientbound_set_inventory_slot set_inventory_slot;
		clientbound_player_pos player_pos;
		clientbound_set_time time_set;
		clientbound_world_reset world_reset;
		clientbound_update_block update_block;
		clientbound_window_transaction window_transaction;
		clientbound_window_open window_open;
		clientbound_spawn_item spawn_item;
		clientbound_pickup_item pickup_item;
		clientbound_entity_destroy entity_destroy;
		clientbound_entity_move entity_move;
	} payload;
} client_rpc;

void clin_init(void);
void clin_update(void);
void clin_rpc_send(client_rpc* call);

#endif
