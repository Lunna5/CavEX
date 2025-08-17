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
#include "../../source/event/event.h"
#include "../../source/log/log.h"

#include <assert.h>

int triggers[] = {0, 0};

void on_key_press_handler_1(const EventContainer* e) {
	log_debug("[Handler 1] KEY_PRESS: key='%c'", e->data.key_press.key);
	triggers[0]++;
}

void on_key_press_handler_2(const EventContainer* e) {
	log_debug("[Handler 2] KEY_PRESS: key='%c'", e->data.key_press.key);
	triggers[1]++;
}

int main(void) {
	log_set_level(LOG_DEBUG);
	event_system_init();
	event_register_handler(EVENT_KEY_PRESS, on_key_press_handler_1);
	event_register_handler(EVENT_KEY_PRESS, on_key_press_handler_2);

	const EventContainer e1 = {.type = EVENT_KEY_PRESS,
						 .length = sizeof(KeyPressData),
						 .data.key_press = {.key = 'A'}};

	log_debug("[EVENT] Triggering EVENT_KEY_PRESS with key = A");
	event_trigger(&e1);

	event_unregister_handler(EVENT_KEY_PRESS, on_key_press_handler_1);

	log_debug("[EVENT] Now only on_key_press_handler_2 should be triggered.");

	event_trigger(&e1); // Should only trigger on_key_press_handler_2

	event_unregister_handler(EVENT_KEY_PRESS, on_key_press_handler_2);

	event_trigger(&e1);

	event_system_cleanup();

	assert(triggers[0] == 1);
	assert(triggers[1] == 2);
}