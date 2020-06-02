#version 330
layout (location = 0) in vec3 a_pos;
uniform vec2 window_size;

out vec4 color;

float x_from_screen_space(float val, float width) {
	return (val * (2/width)) - 1;
}

float y_from_screen_space(float val, float height) {
	return -((val * (2/height)) - 1); // invert for y
}

void main() {
	gl_Position = vec4(
		x_from_screen_space(a_pos.x, window_size.x),
		y_from_screen_space(a_pos.y, window_size.y),
		a_pos.z,
		1.0
	);

	color = vec4(0.1, 1.0, 0.3, 1.0);
}
