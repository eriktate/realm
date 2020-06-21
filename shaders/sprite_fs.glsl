#version 330 core
out vec4 frag_color;

in vec2 tex_coord;
in vec4 color;

uniform sampler2D tex;

void main() {
	frag_color = texture(tex, tex_coord);
	// frag_color = color;
}
