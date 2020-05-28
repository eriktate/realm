#ifndef _GAME_H
#define _GAME_H

#include <stdlib.h>
#include "gm.h"
#include "sprite.h"

typedef struct scene {
	quad *quads;
	sprite *sprites;
	size_t capacity;
	size_t len;
} scene;

typedef struct game {
	scene *current_scene;
	animation *animations;
} game;

scene create_scene(size_t cap);
sprite *scene_add_sprite(scene *sc, sprite spr);

#endif // _GAME_H
