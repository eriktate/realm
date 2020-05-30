#ifndef _TEXTURE_H
#define _TEXTURE_H

typedef struct texture {
	unsigned int id; // OpenGL texture ID
	int width;
	int height;
} texture;

int load_texture(char *fname, texture *tex);

#endif // _TEXTURE_H
