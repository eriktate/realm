#include <stdio.h>
#include "linear.h"

vec2 vec2_new(float x, float y) {
	return (vec2) {
		x,
		y,
	};
}

vec3 vec3_new(float x, float y, float z) {
	return (vec3) {
		x,
		y,
		z,
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

vec4 vec4_new(float x, float y, float z, float w) {
	return (vec4) {
		x,
		y,
		z,
		w,
	};
}

quad quad_new(vec3 tl, vec3 tr, vec3 bl, vec3 br) {
	return (quad){
		tl,
		tr,
		bl,
		br,
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

void print_vec2(vec2 vec) {
	printf("{ x: %.3f, y: %.3f }\n", vec.x, vec.y);
}

void print_vec3(vec3 vec) {
	printf("{ x: %.3f, y: %.3f, z: %.3f }\n", vec.x, vec.y, vec.z);
}

void print_vec4(vec4 vec) {
	printf("{ x: %.3f, y: %.3f, z: %.3f, w: %.3f }\n", vec.x, vec.y, vec.z, vec.w);
}
