#ifndef _GM_H
#define _GM_H

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

typedef struct vertex {
	vec3 pos;
	vec2 tex_coord;
} vertex;

typedef struct quad {
	vertex tl;
	vertex tr;
	vertex bl;
	vertex br;
} quad;

vec2 vec2_new(float, float);
vec3 vec3_new(float, float, float);
vec4 vec4_new(float, float, float, float);
vertex vertex_new(vec3 pos, vec2 tex_coord);
quad quad_new(vertex tl, vertex tr, vertex bl, vertex br);

vec2 vec2_zero();
vec3 vec3_zero();
vec4 vec4_zero();

vec3 swizzle3f2(vec2);
vec4 swizzle4f2(vec2);
vec4 swizzle4f3(vec3);

vec2 vec2_to_clip_space(vec2, float, float);

void print_vec2(vec2);
void print_vec3(vec3);
void print_vec4(vec4);

#endif // _GM_H
