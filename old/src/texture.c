#include <stdbool.h>
#include <GL/gl3w.h>
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int load_texture(char *fname, texture *tex) {
	glGenTextures(1, &(tex->id));
	glBindTexture(GL_TEXTURE_2D, tex->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(fname, &(tex->width), &(tex->height), &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	} else {
		return -1;
	}

	// cleanup
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return 0;
}
