#ifndef _GM_H
#define _GM_H

#include <stdbool.h>
#include "types.h"

typedef struct vec2 {
	f32 x;
	f32 y;
} vec2;

typedef struct vec3 {
	f32 x;
	f32 y;
	f32 z;
} vec3;

typedef struct vec4 {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} vec4;

typedef struct mat4 {
	f32 data[16];
} mat4;

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

typedef struct rect {
	f32 x;
	f32 y;
	f32 width;
	f32 height;
} rect;


vec2 new_vec2(f32, f32);
vec3 new_vec3(f32, f32, f32);
vec4 new_vec4(f32, f32, f32, f32);
vertex new_vertex(vec3 pos, vec2 tex_coord);
quad new_quad(vertex tl, vertex tr, vertex bl, vertex br);
rect new_rect(f32 x, f32 y, f32 width, f32 height);
mat4 identity();
mat4 ortho_proj(f32 l, f32 r, f32 t, f32 b, f32 f, f32 n);

vec2 zero2();
vec3 zero3();
vec4 zero4();

vec3 swizzle3f2(vec2);
vec4 swizzle4f2(vec2);
vec4 swizzle4f3(vec3);

vec3 add_vec3(vec3, vec3);
vec3 sub_vec3(vec3, vec3);

vec2 vec2_to_clip_space(vec2, f32, f32);
vec2 vec2_to_texture_space(vec2, f32, f32);

bool compare_f32(f32 left, f32 right);

f32 mag2(vec2 v);
f32 mag3(vec3 v);
f32 mag4(vec4 v);

vec2 unit2(vec2 v);
vec3 unit3(vec3 v);
vec4 unit4(vec4 v);

vec2 scale2(vec2 v, f32 s);
vec3 scale3(vec3 v, f32 s);
vec4 scale4(vec4 v, f32 s);

mat4 mult_mat4(mat4 l, mat4 r);
mat4 translation(vec3 translate);
mat4 translate_mat4(mat4 m, vec3 translate);

bool overlaps(rect left, rect right);
void translate_rect(rect *r, vec3 v);

// vertex vertex_set_pos(vertex)

void print_vec2(vec2);
void print_vec3(vec3);
void print_vec4(vec4);

void print_vertex(vertex);
void print_quad(quad);

#endif // _GM_H
