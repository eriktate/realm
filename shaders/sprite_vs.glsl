#version 330
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 _tex_coord;
uniform vec2 window_size;
uniform mat4 transform;

out vec2 tex_coord;

void main() {
	gl_Position = transform * vec4(pos, 1.0);

	tex_coord = _tex_coord;
}
