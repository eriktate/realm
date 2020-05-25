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

vec2 new_vec2(float, float);
vec3 new_vec3(float, float, float);
vec4 new_vec4(float, float, float, float);

vec3 swizzle3f2(vec2);
vec4 swizzle4f2(vec2);
vec4 swizzle4f3(vec3);

void print_vec2(vec2);
void print_vec3(vec3);
void print_vec4(vec4);

#endif // _LINEAR_H
