#include <GL/gl3w.h>
#include "gl.h"

u32 create_vao() {
	u32 vao;
	glGenVertexArrays(1, &vao);
	return vao;
}

u32 create_vbo(u32 vao, u32 size, void *data, GLenum usage) {
	u32 vbo;
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

u32 create_scene_vbo(u32 vao, scene sc) {
	return create_vbo(vao, sc.len * sizeof(quad), sc.quads, GL_DYNAMIC_DRAW);
}

u32 create_ebo(u32 vao, elements indices, GLenum usage) {
	u32 ebo;
	glBindVertexArray(vao);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size, indices.data, usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	return ebo;
}

void update_vbo(u32 vao, u32 vbo, u32 size, void *data, GLenum usage) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void update_scene_vbo(u32 vao, u32 vbo, scene sc) {
	update_vbo(vao, vbo, sc.len * sizeof(quad), sc.quads, GL_DYNAMIC_DRAW);
}
