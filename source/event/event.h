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

#ifndef CAVEX_EVENT_H
#define CAVEX_EVENT_H
#include <stdbool.h>

#include <stb_ds.h>
#include "../network/client_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	EVENT_RAW,					// Generic event type for future expansion
	EVENT_KEY_PRESS,
	EVENT_KEY_RELEASE,			// Key release event
	EVENT_CLIENTBOUND_PACKET,
	EVENT_SERVERBOUND_PACKET,
	// UI EVENTS
	UI_EVENT_SLOT_CLICK,        // Left-click on a slot
	UI_EVENT_SLOT_RIGHT_CLICK,  // Right-click on a slot
	UI_EVENT_SLOT_HOVER,        // Mouse over slot
	UI_EVENT_SCREEN_CLOSE,      // Inventory Close
} EventType;

typedef struct {
	char key;
} KeyPressData;

// Event container
typedef struct {
	EventType type;
	size_t length;
	union {
		KeyPressData key_press;
		client_rpc clientbound_packet;
		void* raw;
	} data;
} EventContainer;

typedef void (*EventHandler)(const EventContainer* e);

typedef struct {
	EventType type;
	EventHandler* handlers; // dynamic array
} EventListeners;

void event_system_init(void);
void event_system_cleanup(void);

bool register_event(EventType type, EventHandler handler);
bool unregister_event(EventType type, EventHandler handler);

void trigger_event(const EventContainer* e);

#ifdef __cplusplus
}
#endif
#endif // CAVEX_EVENT_H
