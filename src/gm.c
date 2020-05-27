#include <stdio.h>
#include "gm.h"

vec2 new_vec2(float x, float y) {
	return (vec2) {
		x,
		y,
	};
}

vec3 new_vec3(float x, float y, float z) {
	return (vec3) {
		x,
		y,
		z,
	};
}

vec4 new_vec4(float x, float y, float z, float w) {
	return (vec4) {
		x,
		y,
		z,
		w,
	};
}

vertex new_vertex(vec3 pos, vec2 tex_coord) {
	return (vertex){
		pos,
		tex_coord,
	};
}

quad new_quad(vertex tl, vertex tr, vertex bl, vertex br) {
	return (quad){
		tl,
		tr,
		bl,
		br,
	};
}

vec2 vec2_zero() {
	return (vec2) {
		0.0f,
		0.0f,
	};
}

vec3 vec3_zero() {
	return (vec3) {
		0.0f,
		0.0f,
		0.0f,
	};
}

vec4 vec4_zero() {
	return (vec4) {
		0.0f,
		0.0f,
		0.0f,
		0.0f,
	};
}





vec3 swizzle3f2(vec2 vec) {
	return (vec3){
		vec.x,
		vec.y,
		1.0f,
	};
}

vec4 swizzle4f2(vec2 vec) {
	return (vec4) {
		vec.x,
		vec.y,
		1.0f,
		1.0f,
	};
}

vec4 swizzle4f3(vec3 vec) {
	return (vec4) {
		vec.x,
		vec.y,
		vec.z,
		1.0f,
	};
}

vec3 add_vec3(vec3 left, vec3 right) {
	left.x += right.x;
	left.y += right.y;
	left.z += right.z;

	return left;
}


float x_to_texture_space(float x, float width) {
	return x/width;
}

float y_to_texture_space(float y, float height) {
	return 1-(y/height);
}

float x_to_clip_space(float x, float width) {
	return (x * (2/width)) - 1;
}

float y_to_clip_space(float y, float height) {
	return -((y * (2/height)) - 1); // invert for y
}

float x_to_screen_space(float x, float width) {
	return (2 * (x/width)) - 1;
}

float y_to_screen_space(float y, float height) {
	return -((2 * (y/height)) - 1); // invert for y
}

vec2 vec2_to_clip_space(vec2 v, float width, float height) {
	v.x = x_to_clip_space(v.x, width);
	v.y = y_to_clip_space(v.y, height);

	return v;
}

vec2 vec2_to_texture_space(vec2 v, float width, float height) {
	v.x = x_to_texture_space(v.x, width);
	v.y = y_to_texture_space(v.y, height);

	return v;
}

void print_vec2(vec2 vec) {
	printf("{ x: %.3f, y: %.3f }\n", vec.x, vec.y);
}

void print_vec3(vec3 vec) {
	printf("{ x: %.3f, y: %.3f, z: %.3f }\n", vec.x, vec.y, vec.z);
}

void print_vec4(vec4 vec) {
	printf("{ x: %.3f, y: %.3f, z: %.3f, w: %.3f }\n", vec.x, vec.y, vec.z, vec.w);
}

void print_vertex(vertex vert) {
	printf("Vertex { ");
	print_vec3(vert.pos);
	printf(", ");
	print_vec2(vert.tex_coord);
	printf(" }\n");
}

void print_quad(quad q) {
	printf("Quad { ");
	print_vertex(q.tl);
	print_vertex(q.tr);
	print_vertex(q.bl);
	print_vertex(q.br);
	printf(" }");
	printf("\n");
}
