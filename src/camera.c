#include "camera.h"

camera new_camera(vec3 pos, int width, int height, int inner_width, int inner_height) {
	return (camera){
		.pos = pos,
		.width = width,
		.height = height,
		.inner_width = inner_width,
		.inner_height = inner_height,
	};
}

void look_at(camera *c, vec3 pos) {
	f32 left = c->pos.x - c->inner_width/2;
	f32 right = c->pos.x + c->inner_width/2;
	f32 top = c->pos.y - c->inner_height/2;
	f32 bottom = c->pos.y + c->inner_height/2;

	vec3 new_pos = c->pos;
	if (pos.x < left) {
		new_pos.x = pos.x + c->inner_width/2;
	}

	if (pos.x > right) {
		new_pos.x = pos.x - c->inner_width/2;
	}

	if (pos.y < top) {
		new_pos.y = pos.y + c->inner_height/2;
	}

	if (pos.y > bottom) {
		new_pos.y = pos.y - c->inner_height/2;
	}

	c->pos = new_pos;
}

mat4 get_camera_transform(camera *c) {
	mat4 proj = ortho_proj(0, c->width, 0, c->height, 1, -1);
	mat4 view = translation(new_vec3(c->width/2 - c->pos.x, c->height/2 - c->pos.y, 0));

	return mult_mat4(proj, view);
}
