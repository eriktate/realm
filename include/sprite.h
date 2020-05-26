#ifndef _SPRITE_H
#define _SPRITE_H

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
	int width; // frame width
	int height; // frame height
	int rows; // number of rows for an atlas
	int columns; // numberer of columns for an atlas
} atlas;

// an animation is a configuration of atlas frames to play
typedef struct animation {
	atlas _atlas;
	float speed;
	int *frames;
} animation;

typedef struct sprite {
	vec3 pos;
	int width;
	int height;
	animation anim;

} sprite;

#endif // _SPRITE_H
