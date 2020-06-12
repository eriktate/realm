#include "input.h"

controller new_controller() {
	return (controller){};
}

void clear_inputs(controller *ctrl) {
	for (i32 i = 0; i < INPUT_TYPE_END; i++) {
		ctrl->input_values[i] = 0;
	}
}

void finalize_inputs(controller *ctrl) {
	for (i32 i = 0; i < INPUT_TYPE_END; i++) {
		// an input is currently released
		if (compare_f32(ctrl->input_values[i], 0)) {
			switch (ctrl->input_events[i]) {
			case INPUT_EVENT_PRESSED:
			case INPUT_EVENT_DOWN:
				ctrl->input_events[i] = INPUT_EVENT_RELEASED;
				break;
			default:
				ctrl->input_events[i] = INPUT_EVENT_NONE;
				break;
			}
		} else {
			switch (ctrl->input_events[i]) {
			case INPUT_EVENT_RELEASED:
			case INPUT_EVENT_NONE:
				ctrl->input_events[i] = INPUT_EVENT_PRESSED;
				break;
			default:
				ctrl->input_events[i] = INPUT_EVENT_DOWN;
				break;
			}

		}
	}
}

void set_input(controller *ctrl, input_type type, f32 val) {
	ctrl->input_values[type] = val;
}

f32 get_input(controller *ctrl, input_type type) {
	return ctrl->input_values[type];
}

bool get_input_pressed(controller *ctrl, input_type type) {
	if (ctrl->input_events[type] == INPUT_EVENT_PRESSED) {
		return true;
	}

	return false;
}

bool get_input_released(controller *ctrl, input_type type) {
	if (ctrl->input_events[type] == INPUT_EVENT_RELEASED) {
		return true;
	}

	return false;
}
