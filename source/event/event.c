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

#include "event.h"

static EventListeners* g_event_listeners = NULL;
void event_system_init(void) {
	g_event_listeners = NULL; // arrlen = 0
}

void event_system_cleanup(void) {
	for (size_t i = 0; i < arrlen(g_event_listeners); i++) {
		arrfree(g_event_listeners[i].handlers);
	}
	arrfree(g_event_listeners);
}

static EventListeners* get_or_create_listeners(const EventType type) {
	for (size_t i = 0; i < arrlen(g_event_listeners); i++) {
		EventListeners* el = &g_event_listeners[i];
		if (el->type == type) return el;
	}

	EventListeners el;
	el.type = type;
	el.handlers = NULL;
	arrput(g_event_listeners, el);
	return &g_event_listeners[arrlen(g_event_listeners)-1];
}

bool event_register_handler(const EventType type, const EventHandler handler) {
	if (!handler) return false;

	EventListeners* el = get_or_create_listeners(type);
	arrput(el->handlers, handler);
	return true;
}

bool event_unregister_handler(const EventType type, const EventHandler handler) {
	if (!handler) return false;
	for (size_t i = 0; i < arrlen(g_event_listeners); i++) {
		EventListeners* el = &g_event_listeners[i];
		if (el->type == type) {
			for (size_t j = 0; j < arrlen(el->handlers); j++) {
				if (el->handlers[j] == handler) {
					arrdel(el->handlers, j);
					return true;
				}
			}
		}
	}
	return false;
}

void event_trigger(const EventContainer* e) {
	if (!e || !g_event_listeners) return;

	for (size_t i = 0; i < arrlen(g_event_listeners); i++) {
		const EventListeners* el = &g_event_listeners[i];
		if (el->type == e->type) {
			for (size_t j = 0; j < arrlen(el->handlers); j++) {
				const EventHandler handler = el->handlers[j];

				if (handler) {
					handler(e);
				}
			}
		}
	}
}
