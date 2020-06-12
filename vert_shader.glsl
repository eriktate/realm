#version 330
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 _tex_coord;

out vec2 tex_coord;
out vec4 color;

void main() {
	gl_Position = vec4(a_pos, 1.0);
	tex_coord = _tex_coord;
	color = vec4(1.0, 0.0, 0.0, 1.0);
}
