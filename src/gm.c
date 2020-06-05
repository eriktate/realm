#include <stdio.h>
#include <math.h>
#include "gm.h"

vec2 new_vec2(f32 x, f32 y) {
	return (vec2) {
		x,
		y,
	};
}

vec3 new_vec3(f32 x, f32 y, f32 z) {
	return (vec3) {
		x,
		y,
		z,
	};
}

vec4 new_vec4(f32 x, f32 y, f32 z, f32 w) {
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

rect new_rect(f32 x, f32 y, f32 w, f32 h) {
	return (rect){
		.x = x,
		.y = y,
		.width = w,
		.height = h,
	};
}

mat4 identity() {
	return (mat4){
		.data = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		},
	};
}

mat4 ortho_proj(f32 l, f32 r, f32 t, f32 b, f32 f, f32 n) {
	return (mat4){
		.data = {
			2/(r-l),      0,            0,            0,
			0,            2/(t-b),      0,            0,
			0,            0,            -2/(f-n),     0,
			-(r+l)/(r-l), -(t+b)/(t-b), -(f+n)/(f-n), 1,
		}
	};
}

vec2 zero2() {
	return (vec2) {
		0.0f,
		0.0f,
	};
}

vec3 zero3() {
	return (vec3) {
		0.0f,
		0.0f,
		0.0f,
	};
}

vec4 zero4() {
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

vec3 sub_vec3(vec3 left, vec3 right) {
	left.x -= right.x;
	left.y -= right.y;
	left.z -= right.z;

	return left;
}

static f32 y_to_texture_space(f32 y, f32 height) {
	return 1-(y/height);
}

static f32 x_to_texture_space(f32 x, f32 width) {
	return x/width;
}

vec2 vec2_to_texture_space(vec2 v, f32 width, f32 height) {
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
		return zero2();
	}

	return new_vec2(v.x/mag, v.y/mag);
}

vec3 unit3(vec3 v) {
	f32 mag = mag3(v);
	if (compare_f32(mag, 0.0f)) {
		return zero3();
	}

	return new_vec3(v.x/mag, v.y/mag, v.z/mag);
}

vec4 unit4(vec4 v) {
	f32 mag = mag4(v);
	if (compare_f32(mag, 0.0f)) {
		return zero4();
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

/* Column major matrix
 * -------------------
 * 0, 4, 8, 12
 * 1, 5, 9, 13,
 * 2, 6, 10, 14,
 * 3, 7, 11, 15,
*/
mat4 mult_mat4(mat4 l, mat4 r) {
	mat4 res;
	for (i32 i = 0; i < 4; i++) {
		for (i32 j = 0; j < 4; j++) {
			res.data[i+(4*j)] =
				(l.data[i] * r.data[j*4]) +
				(l.data[(i+4)] * r.data[(j*4)+1]) +
				(l.data[(i+8)] * r.data[(j*4)+2]) +
				(l.data[(i+12)] * r.data[(j*4)+3]);
		}
	}

	return res;
}

mat4 translation(vec3 translate) {
	mat4 m = identity();
	m.data[12] = translate.x;
	m.data[13] = translate.y;
	m.data[14] = translate.z;

	return m;
}

mat4 translate_mat4(mat4 m, vec3 translate) {
	mat4 t = translation(translate);
	return mult_mat4(m, t);
}

static bool _overlaps(rect l, rect r) {
	bool y_overlap = (
		(l.y >= r.y && l.y <= (r.y + r.height)) ||
		((l.y + l.height) >= r.y && (l.y + l.height) <= (r.y + r.height))
	);

	bool x_overlap = (
		(l.x <= (r.x + r.width) && l.x >= r.x) ||
		((l.x + l.width) <= (r.x + r.width) && ((l.x + l.width) >= r.x))
	);

	return y_overlap && x_overlap;
}

bool overlaps(rect l, rect r) {
	// flipping the args should detect containment
	return  _overlaps(l, r) || _overlaps(r, l);
}

void translate_rect(rect *r, vec3 v) {
	r->x = v.x;
	r->y = v.y;
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
