#ifndef _GL_H
#define _GL_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "types.h"
#include "game.h"

typedef struct elements {
	size_t len;
	size_t size;
	u32 *data;
} elements;

GLFWwindow *create_window(u32 width, u32 height, char *title);

u32 create_vao();
u32 create_vbo(u32 vao, u32 size, void *data, GLenum usage);
u32 create_scene_vbo(u32 vao, scene sc);
u32 create_ebo(u32 vao, elements indices, GLenum usage);

void update_vbo(u32 vao, u32 vbo, u32 size, void *data, GLenum usage);
void update_scene_vbo(u32 vao, u32 vbo, scene sc);

#endif // _GL_H
