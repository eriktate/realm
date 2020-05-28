#include <stdio.h>
#include "sprite.h"
#include "gm.h"

const unsigned int TEX_WIDTH = 512;
const unsigned int TEX_HEIGHT = 512;

quad get_quad_from_sprite(sprite *spr) {
	vec3 pos = spr->pos;
	return new_quad(
		new_vertex(
			pos,
			vec2_to_texture_space(new_vec2(128.0f, 160.0f), TEX_WIDTH, TEX_HEIGHT)
		),
		new_vertex(
			new_vec3(pos.x + spr->width, pos.y, pos.z),
			vec2_to_texture_space(new_vec2(144.0f, 160.0f), TEX_WIDTH, TEX_HEIGHT)
		),
		new_vertex(
			new_vec3(pos.x, pos.y + spr->height, pos.z),
			vec2_to_texture_space(new_vec2(128.0f, 192.f), TEX_WIDTH, TEX_HEIGHT)
		),
		new_vertex(
			new_vec3(pos.x + spr->width, pos.y + spr->height, pos.z),
			vec2_to_texture_space(new_vec2(144.0f, 192.0f), TEX_WIDTH, TEX_HEIGHT)
		)
	);
}

sprite new_sprite(vec3 pos, int width, int height) {
	return (sprite){
		0,
		pos,
		width,
		height,
		0
	};
}

void set_sprite_pos(sprite *spr, vec3 pos) {
	spr->pos = pos;
	quad *q = spr->_quad;

	// need to take reference to nested vertex struct in order to just update
	// the pos vec
	(&q->tl)->pos = pos;
	(&q->tr)->pos = new_vec3(pos.x + spr->width, pos.y, pos.z);
	(&q->bl)->pos = new_vec3(pos.x, pos.y + spr->height, pos.z);
	(&q->br)->pos = new_vec3(pos.x + spr->width, pos.y + spr->height, pos.z);
}
