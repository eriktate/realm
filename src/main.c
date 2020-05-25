#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "linear.h"
#include "file.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int WIDTH = 640;
const int HEIGHT = 480;

void process_input(GLFWwindow *w) {
	if (glfwGetKey(w, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(w, true);
	}
}

typedef struct index_arr {
	size_t len;
	size_t size;
	unsigned int *indices;
} index_arr;

// brute force indices for a number of quads
index_arr generate_indices(int quad_count) {
	size_t len = quad_count * 6;
	size_t size = len * sizeof(unsigned int);

	unsigned int *indices = (unsigned int *)malloc(size);
	for (int i = 0; i < quad_count; i++) {
		unsigned int tl = i * 4;
		unsigned int tr = i * 4 +1;
		unsigned int bl = i * 4 + 2;
		unsigned int br = i * 4 + 3;

		indices[i*6] = tl;
		indices[i*6+1] = tr;
		indices[i*6+2] = bl;
		indices[i*6+3] = tr;
		indices[i*6+4] = bl;
		indices[i*6+5] = br;
	}

	return (index_arr){
		len,
		size,
		indices,
	};
}

void print_index_arr(index_arr arr) {
	printf("[ ");
	for (int i = 0; i < arr.len; i++) {
		printf("%d, ", arr.indices[i]);
	}

	printf(" ]\n");
}

int main(void)
{

	GLFWwindow* window;

	if (!glfwInit()) {
		printf("glfw init failed\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "playground", NULL, NULL);
	if (!window)
	{
		printf("window creation failed\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (gl3wInit()) {
		printf("gl3w init failed\n");
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	// load texture
	// TODO (etate): Load texture into vram

	quad quads[] = {
		quad_new(
			vec3_new(0.0,  0.0f, 0.0f), // top left
			vec3_new(32.0f, 0.0f, 0.0f), // top right
			vec3_new(0.0f, 32.0f, 0.0f), // bottom left
			vec3_new(32.0, 32.0f, 0.0f) // bottom right
		),
	};


	index_arr indices = generate_indices(sizeof(quads) / sizeof(quad));
	print_index_arr(indices);

	// generate VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// generate vertex buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quads), quads, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	// generate element buffer
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size, indices.indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	// unbind VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// load shaders
	const char *vert_source = read_file("vert_shader.glsl");
	const char *frag_source = read_file("frag_shader.glsl");

	unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vert_source, NULL);
	glCompileShader(vert_shader);

	unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &frag_source, NULL);
	glCompileShader(frag_shader);

	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program);

	glUseProgram(shader_program);
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	int uniform_window_size = glGetUniformLocation(shader_program, "window_size");
	glUniform2f(uniform_window_size, (float)WIDTH, (float)HEIGHT);

	while (!glfwWindowShouldClose(window))
	{
		process_input(window);

		glClearColor(0.5, 0.3, 0.8, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.len, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
