#include <stdio.h>
#include "gl.h"

#define MAJOR_VERSION 3
#define MINOR_VERSION 3

GLFWwindow *create_window(u32 width, u32 height, char *title) {
	GLFWwindow *window;

	if (!glfwInit()) {
		printf("glfw init failed\n");
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSwapInterval(0); // turn off vsync if left up to the game
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
	{
		printf("window creation failed\n");
		glfwTerminate();
		return NULL;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwMakeContextCurrent(window);

	if (gl3wInit()) {
		printf("gl3w init failed\n");
		return NULL;
	}

	glViewport(0, 0, width, height);

	return window;
}

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
	glBindVertexArray(0);
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
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return ebo;
}

void update_vbo(u32 vao, u32 vbo, u32 size, void *data, GLenum usage) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void update_scene_vbo(u32 vao, u32 vbo, scene sc) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sc.len * sizeof(quad), sc.quads, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)sizeof(vec3));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
