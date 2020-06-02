#ifndef _INPUT_H
#define _INPUT_H

#include "types.h"
#include "gm.h"

typedef enum input_type {
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_UP,
	INPUT_DOWN,
	INPUT_END,
} input_type;

typedef struct controller {
	vec2 cursor_pos;
	f32 inputs[INPUT_END];
} controller;

controller new_controller();
void clear_inputs(controller *ctrl);
void set_input(controller *ctrl, input_type type, f32 val);
f32 get_input(controller *ctrl, input_type type);

#endif // _INPUT_H
