#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "gm.h"
#include "sprite.h"

void grow_capacity(scene *sc) {
	// only grow if we need to
	if (sc->len < sc->capacity) {
		return;
	}

	// double the capacity
	size_t new_cap = sc->capacity * 2;
	quad *new_quads = malloc(new_cap * sizeof(quad));
	sprite *new_sprites = malloc(new_cap * sizeof(sprite));

	// copy original data
	memcpy(new_quads, sc->quads, sc->len * sizeof(quad));
	memcpy(new_sprites, sc->sprites, sc->len * sizeof(sprite));

	// free old arrays
	free(sc->quads);
	free(sc->sprites);

	// assign new arrays
	sc->quads = new_quads;
	sc->sprites = new_sprites;
}

scene create_scene(size_t cap) {
	quad *quads = (quad *)malloc(cap * sizeof(quad));
	sprite *sprites = (sprite *)malloc(cap * sizeof(sprite));

	return (scene){
		quads,
		sprites,
		cap,
		0
	};
}

sprite *scene_add_sprite(scene *sc, sprite spr) {
	spr.id = sc->len;
	sc->sprites[sc->len] = spr;
	sc->quads[sc->len] = get_quad_from_sprite(&spr);
	sc->len++;

	sprite *added = &(sc->sprites[(sc->len)-1]);
	added->_quad = &(sc->quads[sc->len-1]);
	grow_capacity(sc);

	return added;
}
