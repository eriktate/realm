#include <stdio.h>
#include <stdlib.h>
#include <GL/gl3w.h>
#include "file.h"

unsigned int load_shader_program(char *vert_name , char *frag_name) {
	char *vert_source = read_file(vert_name);
	char *frag_source = read_file(frag_name);

	unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vert_source, NULL);
	glCompileShader(vert_shader);

	int success;
	char info_log[512];
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
		printf("vertex shader compilation failed: %s\n", info_log);
	}

	unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &frag_source, NULL);
	glCompileShader(frag_shader);
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
		printf("fragment shader compilation failed: %s\n", info_log);
	}

	unsigned int program_id = glCreateProgram();
	glAttachShader(program_id, vert_shader);
	glAttachShader(program_id, frag_shader);
	glLinkProgram(program_id);

	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(program_id, 512, NULL, info_log);
		printf("shader linking failed: %s\n", info_log);
	}

	glUseProgram(program_id);
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	free(vert_source);
	free(frag_source);

	return program_id;
}