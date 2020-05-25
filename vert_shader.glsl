#version 330
layout (location = 0) in vec3 a_pos;
uniform vec2 window_size;

float x_from_screen_space(float val) {
	return (val * (2/window_size.x)) - 1;
}

float y_from_screen_space(float val) {
	return -((val * (2/window_size.y)) - 1); // invert for y
}

void main() {
	gl_Position = vec4(x_from_screen_space(a_pos.x), y_from_screen_space(a_pos.y), a_pos.z, 1.0);
	// gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
}
