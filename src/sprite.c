#include <stdio.h>
#include "sprite.h"

const unsigned int TEX_WIDTH = 512;
const unsigned int TEX_HEIGHT = 512;

tex_quad get_current_frame(animation *anim) {
	return anim->frames[(i32)anim->current_frame];
}

quad get_quad_from_sprite(sprite *spr) {
	printf("Get quad from sprite\n");
	vec3 pos = spr->pos;
	tex_quad frame;
	if (spr->animated) {
		frame = get_current_frame(&spr->anim);
	} else {
		frame = spr->tex;
	}

	return new_quad(
		new_vertex(
			pos,
			frame.tl
		),
		new_vertex(
			new_vec3(pos.x + spr->width, pos.y, pos.z),
			frame.tr
		),
		new_vertex(
			new_vec3(pos.x, pos.y + spr->height, pos.z),
			frame.bl
		),
		new_vertex(
			new_vec3(pos.x + spr->width, pos.y + spr->height, pos.z),
			frame.br
		)
	);
}

static void animate(animation *anim, f64 delta) {
	anim->current_frame += (anim->framerate * delta);
	if (anim->current_frame >= anim->frame_count) {
		// TODO (etate): Potential edge case. If enough time goes by
		// between renders, we could still exceed the frame_count after
		// this operation.
		anim->current_frame = anim->current_frame - anim->frame_count;
	}
}

void sprite_animate(sprite *spr, f64 delta) {
	// short circuit if not animated
	if (!spr->animated) {
		return;
	}

	animation *anim = &spr->anim;
	i32 previous_frame = (i32)anim->current_frame;
	animate(anim, delta);

	// short circuit if the frame hasn't changed
	if ((i32)anim->current_frame == previous_frame) {
		return;
	}

	// update sprite quad with new texture coordinates
	tex_quad new_tex = anim->frames[(i32)anim->current_frame];
	spr->_quad->tl.tex_coord = new_tex.tl;
	spr->_quad->tr.tex_coord = new_tex.tr;
	spr->_quad->bl.tex_coord = new_tex.bl;
	spr->_quad->br.tex_coord = new_tex.br;
}

sprite new_sprite(vec3 pos, int width, int height, bool animated) {
	return (sprite){
		.id = 0,
		.pos = pos,
		.width = width,
		.height = height,
		.animated = animated,
		.hitbox = new_rect(0, 0, width, height), // default hitbox matches sprite dimensions
		._quad = NULL,
	};
}

// generate relative hitbox
rect get_hitbox(sprite *spr) {
	return new_rect(spr->pos.x + spr->hitbox.x, spr->pos.y + spr->hitbox.y, spr->hitbox.width, spr->hitbox.height);
}

rect get_potential_hitbox(sprite *spr, vec3 pos) {
	return new_rect(pos.x + spr->hitbox.x, pos.y + spr->hitbox.y, spr->hitbox.width, spr->hitbox.height);
}

void set_sprite_pos(sprite *spr, vec3 pos) {
	spr->pos = pos;
	quad *q = spr->_quad;

	q->tl.pos = pos;
	q->tr.pos = new_vec3(pos.x + spr->width, pos.y, pos.z);
	q->bl.pos = new_vec3(pos.x, pos.y + spr->height, pos.z);
	q->br.pos = new_vec3(pos.x + spr->width, pos.y + spr->height, pos.z);
}

vec2 new_tex_coord(f32 x, f32 y, i32 width, i32 height) {
	return vec2_to_texture_space(new_vec2(x, y), width, height);
}

animation new_animation(atlas atl, f32 framerate, i32 frame_count, int *frames) {
	tex_quad *frame_quads = (tex_quad *)malloc(sizeof(tex_quad) * frame_count);

	for (i32 i = 0; i < frame_count; i++) {
		frame_quads[i] = atl.frames[frames[i]];
	}

	return (animation){
		.framerate = framerate,
		.frame_count = frame_count,
		.frames = frame_quads,
	};
}

atlas new_atlas(texture tex, i32 offset_x, i32 offset_y, i32 width, i32 height, i32 x_sep, i32 y_sep, i32 rows, i32 cols) {
	tex_quad *frames = (tex_quad *)malloc(sizeof(tex_quad) * (rows * cols));
	i32 count = 0;
	for (i32 r = 0; r < rows; r++) {
		for (i32 c = 0; c < cols; c++) { // lol c++
			vec2 tl = new_vec2(offset_x + (x_sep * (c + 1) + (c * width)), offset_y + (y_sep * (r + 1)) + (r * height));
			tex_quad frame = (tex_quad){
				.tl = vec2_to_texture_space(tl, tex.width, tex.height),
				.tr = new_tex_coord(tl.x + width, tl.y, tex.width, tex.height),
				.bl = new_tex_coord(tl.x, tl.y + height, tex.width, tex.height),
				.br = new_tex_coord(tl.x + width, tl.y + height, tex.width, tex.height),
			};

			frames[count++] = frame;
		}
	}

	return (atlas){
		.tex = tex,
		.offset_x = offset_x,
		.offset_y = offset_y,
		.width = width,
		.height = height,
		.x_sep = x_sep,
		.y_sep = y_sep,
		.rows = rows,
		.columns = cols,
		.frames = frames,
	};
}
