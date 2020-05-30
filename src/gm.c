#include <stdio.h>
#include <math.h>
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

bool compare_f32(f32 left, f32 right) {
	return left > (right - 0.00001) && left < (right + 0.00001);
}

f32 mag2(vec2 v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

f32 mag3(vec3 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

f32 mag4(vec4 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

vec2 unit2(vec2 v) {
	f32 mag = mag2(v);
	if (compare_f32(mag, 0.0f)) {
		return vec2_zero();
	}

	return new_vec2(v.x/mag, v.y/mag);
}

vec3 unit3(vec3 v) {
	f32 mag = mag3(v);
	if (compare_f32(mag, 0.0f)) {
		return vec3_zero();
	}

	return new_vec3(v.x/mag, v.y/mag, v.z/mag);
}

vec4 unit4(vec4 v) {
	f32 mag = mag4(v);
	if (compare_f32(mag, 0.0f)) {
		return vec4_zero();
	}

	return new_vec4(v.x/mag, v.y/mag, v.z/mag, v.w/mag);
}

vec2 scale2(vec2 v, f32 s) {
	return new_vec2(v.x * s, v.y * s);
}

vec3 scale3(vec3 v, f32 s) {
	return new_vec3(v.x * s, v.y * s, v.z * s);
}

vec4 scale4(vec4 v, f32 s) {
	return new_vec4(v.x * s, v.y * s, v.z * s, v.w * s);
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
