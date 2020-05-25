#include <stdio.h>
#include <stdbool.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "linear.h"
#include "file.h"

const int WIDTH = 640;
const int HEIGHT = 480;

const char *vert_source = \
	"#version 330 core\n" \
	"layout (location = 0) in vec3 a_pos;\n" \
	"void main() {\n" \
	"gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n" \
	"}";

const char *frag_source = \
	"#version 330 core\n" \
	"out vec4 frag_color;\n" \
	"void main() {\n" \
	"frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" \
	"}";

void process_input(GLFWwindow *w) {
	if (glfwGetKey(w, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(w, true);
	}
}

int main(void)
{

	char *shader = read_file("vert_shader.glsl");
	return 0;
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

	// set up drawing
	vec3 vertices[] = {
		new_vec3(-0.5f, -0.5f, 0.0f),
		new_vec3(0.5f, -0.5f, 0.0f),
		new_vec3(0.0f,  0.5f, 0.0f),
	};

	// generate VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// generate vertex buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
	glEnableVertexAttribArray(0);

	// unbind VAO
	glBindVertexArray(0);

	// load shaders
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

	while (!glfwWindowShouldClose(window))
	{
		process_input(window);

		glClearColor(0.5, 0.3, 0.8, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
