#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "types.h"

typedef struct texture {
	u32 id; // OpenGL texture ID
	i32 width;
	i32 height;
} texture;

int load_texture(char *fname, texture *tex);

#endif // _TEXTURE_H
