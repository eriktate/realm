#ifndef _SPRITE_H
#define _SPRITE_H

#include <stdbool.h>
#include <stdlib.h>
#include "types.h"
#include "gm.h"
#include "texture.h"


// a quad representing a frame within a texture
typedef struct tex_quad {
	vec2 tl;
	vec2 tr;
	vec2 bl;
	vec2 br;
} tex_quad;

// an atlas is a specific region within a texture divided into frames
typedef struct atlas {
	texture tex; // OpenGL texture ID
	vec2 offset;
	i32 width; // frame width
	i32 height; // frame height
	i32 rows; // number of rows for an atlas
	i32 columns; // numberer of columns for an atlas
} atlas;

// an animation is a configuration of frames to play
typedef struct animation {
	u32 id;
	f32 framerate;
	f32 current_frame;
	i32 frame_count;
	tex_quad *frames;
} animation;

typedef enum anim_id {
	ANIM_PLAYER_IDLE,
	ANIM_PLAYER_WALK,
} anim_id;

typedef struct sprite {
	u32 id; // id assigned by the scene

	// orientation
	vec3 pos;
	i32 width;
	i32 height;

	// animation or texture to render
	bool animated;
	union {
		animation anim;
		tex_quad tex;
	};

	quad *_quad;
} sprite;

quad get_quad_from_sprite(sprite *spr);
tex_quad get_current_frame(animation *anim);

sprite new_sprite(vec3 pos, i32 width, i32 height, bool animated);

// sprite setters will also update the underlying quad
void set_sprite_pos(sprite *spr, vec3 pos);
void sprite_animate(sprite *spr, f64 delta);

vec2 new_tex_coord(f32 x, f32 y, i32 width, i32 height);

// TODO (etate): Temporary helper
animation new_animation(f32 framerate, i32 frame_count, vec2 offset, i32 frame_width, i32 frame_height, i32 tex_width, i32 tex_height);

#endif // _SPRITE_H
