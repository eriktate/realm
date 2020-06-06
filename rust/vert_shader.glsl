#version 330
layout (location = 0) in vec3 a_pos;

out vec4 color;

void main() {
	gl_Position = vec4(a_pos, 1.0);

	color = vec4(1.0, 0.3, 0.3, 1.0);
}
