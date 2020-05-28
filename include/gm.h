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

vec2 new_vec2(float, float);
vec3 new_vec3(float, float, float);
vec4 new_vec4(float, float, float, float);
vertex new_vertex(vec3 pos, vec2 tex_coord);
quad new_quad(vertex tl, vertex tr, vertex bl, vertex br);

vec2 vec2_zero();
vec3 vec3_zero();
vec4 vec4_zero();

vec3 swizzle3f2(vec2);
vec4 swizzle4f2(vec2);
vec4 swizzle4f3(vec3);

vec3 add_vec3(vec3, vec3);

vec2 vec2_to_clip_space(vec2, float, float);
vec2 vec2_to_texture_space(vec2, float, float);

// vertex vertex_set_pos(vertex)

void print_vec2(vec2);
void print_vec3(vec3);
void print_vec4(vec4);

void print_vertex(vertex);
void print_quad(quad);

#endif // _GM_H
