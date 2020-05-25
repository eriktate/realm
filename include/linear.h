#ifndef _LINEAR_H
#define _LINEAR_H

typedef struct vec2 {
	float x;
	float y;
} vec2;

typedef struct vec3 {
	float x;
	float y;
	float z;
} vec3;

typedef struct vec4 {
	float x;
	float y;
	float z;
	float w;
} vec4;

typedef struct quad {
	vec3 tl;
	vec3 tr;
	vec3 bl;
	vec3 br;
} quad;

vec2 vec2_new(float, float);
vec3 vec3_new(float, float, float);
vec4 vec4_new(float, float, float, float);
quad quad_new(vec3 tl, vec3 tr, vec3 bl, vec3 br);

vec2 vec2_zero();
vec3 vec3_zero();
vec4 vec4_zero();

vec3 swizzle3f2(vec2);
vec4 swizzle4f2(vec2);
vec4 swizzle4f3(vec3);

void print_vec2(vec2);
void print_vec3(vec3);
void print_vec4(vec4);

#endif // _LINEAR_H
