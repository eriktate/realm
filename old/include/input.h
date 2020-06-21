#ifndef _INPUT_H
#define _INPUT_H

#include "types.h"
#include "gm.h"

typedef enum input_type {
	INPUT_TYPE_LEFT,
	INPUT_TYPE_RIGHT,
	INPUT_TYPE_UP,
	INPUT_TYPE_DOWN,
	INPUT_TYPE_DEBUG,
	INPUT_TYPE_END, // must be last
} input_type;

typedef enum input_event {
	INPUT_EVENT_NONE,
	INPUT_EVENT_PRESSED,
	INPUT_EVENT_RELEASED,
	INPUT_EVENT_DOWN,
	INPUT_EVENT_END, // must be last
} input_event;

typedef struct controller {
	vec2 cursor_pos;
	f32 input_values[INPUT_TYPE_END];
	input_event input_events[INPUT_TYPE_END];
} controller;

controller new_controller();
void clear_inputs(controller *ctrl);
void finalize_inputs(controller *ctrl);
void set_input(controller *ctrl, input_type type, f32 val);
f32 get_input(controller *ctrl, input_type type);
bool get_input_pressed(controller *ctrl, input_type type);
bool get_input_released(controller *ctrl, input_type type);

#endif // _INPUT_H
