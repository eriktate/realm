#version 330
layout (location = 0) in vec3 _pos;
layout (location = 1) in vec2 _tex_coord;
uniform mat4 transform;

out vec2 tex_coord;
out vec4 color;

void main() {
	gl_Position = transform * vec4(_pos, 1.0);

	tex_coord = _tex_coord;
	color = vec4(1.0, 0.0, 0.0, 1.0);
}
