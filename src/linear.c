#include <stdio.h>
#include "linear.h"

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
