#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "gm.h"
#include "file.h"
#include "shader.h"
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

void show_fps(GLFWwindow *w, int fps) {
	char title[128];

	sprintf(title, "Playground: %d", fps);
	glfwSetWindowTitle(w, title);
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
	glfwSwapInterval(0); // turn off vsync if left up to the game
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
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("mario.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		printf("failed to load texture\n");
		return 1;
	}
	stbi_image_free(data);

	quad quads[] = {
		quad_new(
			// top left
			vertex_new(
				vec3_new(0.0f,  0.0f, 0.0f),
				vec2_new(0.0f, 1.0f)
				// vec2_to_clip_space(vec2_new(0.0f, 0.0f), width, height)
			),
			// top right
			vertex_new(
				vec3_new(256.0f, 0.0f, 0.0f),
				vec2_new(1.0f, 1.0f)
				// vec2_to_clip_space(vec2_new(420.0f, 0.0f), width, height)
			),
			// bottom left
			vertex_new(
				vec3_new(0.0f, 256.0f, 0.0f),
				vec2_new(0.0f, 0.0f)
				// vec2_to_clip_space(vec2_new(0.0f, 420.f), width, height)
			),
			// bottom right
			vertex_new(
				vec3_new(256.0f, 256.0f, 0.0f),
				vec2_new(1.0f, 0.0f)
				// vec2_to_clip_space(vec2_new(420.0f, 420.0f), width, height)
			)
		),
	};

	index_arr indices = generate_indices(sizeof(quads) / sizeof(quad));

	// generate VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// generate vertex buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quads), quads, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)sizeof(vec3));

	// generate element buffer
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size, indices.indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// unbind VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// load shaders
	unsigned int shader_id = load_shader_program("vert.glsl", "frag.glsl");
	int uniform_window_size = glGetUniformLocation(shader_id, "window_size");
	glUniform2f(uniform_window_size, (float)WIDTH, (float)HEIGHT);
	glUniform1i(glGetUniformLocation(shader_id, "tex"), GL_TEXTURE0);

	// alpha in texture won't work without setting the blend mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	double last_time = glfwGetTime();
	double elapsed = 0.0;
	int frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		process_input(window);
		glClearColor(0.5f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.len, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
		frames++;
		if (glfwGetTime() - last_time > 1.0) {
			show_fps(window, frames);
			frames = 0;
			last_time = glfwGetTime();
		}

	}

	glfwTerminate();
	return 0;
}
