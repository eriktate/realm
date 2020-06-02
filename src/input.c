#include "input.h"

controller new_controller() {
	return (controller){};
}

void clear_inputs(controller *ctrl) {
	for (i32 i = 0; i < INPUT_END; i++) {
		ctrl->inputs[i] = 0;
	}
}

void set_input(controller *ctrl, input_type type, f32 val) {
	ctrl->inputs[type] = val;
}

f32 get_input(controller *ctrl, input_type type) {
	return ctrl->inputs[type];
}
