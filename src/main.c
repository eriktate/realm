#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "types.h"
#include "gm.h"
#include "file.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "game.h"

const i32 WIDTH = 640;
const i32 HEIGHT = 480;
vec3 player_pos;

void process_input(GLFWwindow *w) {
	if (glfwGetKey(w, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(w, true);
	}

	vec3 move_vec = new_vec3(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(w, GLFW_KEY_D)) {
		move_vec.x = 1.0f;
	} else if (glfwGetKey(w, GLFW_KEY_A)) {
		move_vec.x = -1.0f;
	}

	if (glfwGetKey(w, GLFW_KEY_W)) {
		move_vec.y = -1.0f;
	} else if (glfwGetKey(w, GLFW_KEY_S)) {
		move_vec.y = 1.0f;
	}

	// normalize and scale to move speed
	move_vec = scale3(unit3(move_vec), 4.0f);
	vec3 new_vec = add_vec3(player_pos, move_vec);
	rect player_rect = new_rect(new_vec.x, new_vec.y, 128, 256);
	rect lady_rect = new_rect(256, 128, 128, 256);
	if (overlaps(player_rect, lady_rect)) {
		vec3 initial_vec = new_vec;
		new_vec.x = player_pos.x;
		player_rect = new_rect(new_vec.x, new_vec.y, 128, 256);
		if (overlaps(player_rect, lady_rect)) {
			new_vec = initial_vec;
			new_vec.y = player_pos.y;
			player_rect = new_rect(new_vec.x, new_vec.y, 128, 256);
			if (overlaps(player_rect, lady_rect)) {
				return;
			}
		}
	}

	player_pos = new_vec;
}

typedef struct index_arr {
	size_t len;
	size_t size;
	u32 *indices;
} index_arr;

// brute force indices for a number of quads
index_arr generate_indices(i32 quad_count) {
	size_t len = quad_count * 6;
	size_t size = len * sizeof(u32);

	u32 *indices = (u32 *)malloc(size);
	for (i32 i = 0; i < quad_count; i++) {
		u32 tl = i * 4;
		u32 tr = i * 4 +1;
		u32 bl = i * 4 + 2;
		u32 br = i * 4 + 3;

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
	for (i32 i = 0; i < arr.len; i++) {
		printf("%d, ", arr.indices[i]);
	}

	printf(" ]\n");
}

void show_fps(GLFWwindow *w, i32 fps) {
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
	window = glfwCreateWindow(WIDTH, HEIGHT, "float - playground", NULL, NULL);
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
	texture tex;
	if (load_texture("spritesheet.png", &tex)) {
		printf("failed to load texture\n");
		return -1;
	}

	atlas sprite_atlas = new_atlas(tex, 128, 128, 16, 32, 0, 0, 3, 8);

	player_pos = new_vec3(128.0f, 128.0f, 0.0f);
	sprite player_meta = new_sprite(player_pos, 128, 256, true);
	sprite lady_meta = new_sprite(new_vec3(256.0f, 128.0f, 0.0f), 128, 256, true);

	int player_frames[8] = {8, 9, 10, 11, 12, 13, 14, 15};
	int lady_frames[8] = {0, 1, 2, 3, 4, 5, 6, 7};

	player_meta.anim = new_animation(sprite_atlas, 6, 3, player_frames);
	lady_meta.anim = new_animation(sprite_atlas, 6, 3, lady_frames);

	scene sc = create_scene(5);
	sprite *player = scene_add_sprite(&sc, player_meta);
	sprite *lady = scene_add_sprite(&sc, lady_meta);

	index_arr indices = generate_indices(sc.len);

	// generate VAO
	u32 VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// generate vertex buffer
	u32 VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sc.len * sizeof(quad), sc.quads, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)sizeof(vec3));

	// generate element buffer
	u32 EBO;
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
	u32 shader_id = load_shader_program("vert.glsl", "frag.glsl");
	i32 uniform_window_size = glGetUniformLocation(shader_id, "window_size");
	glUniform2f(uniform_window_size, (f32)WIDTH, (f32)HEIGHT);
	glUniform1i(glGetUniformLocation(shader_id, "tex"), GL_TEXTURE0);

	// alpha in texture won't work without setting the blend mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	f64 last_frame = glfwGetTime();
	f64 last_second = glfwGetTime();
	i32 frames = 0;
	f64 delta = 0;

	while (!glfwWindowShouldClose(window))
	{
		// update
		delta = glfwGetTime() - last_frame;
		last_frame = glfwGetTime();
		process_input(window);
		// animate
		for (i32 i = 0; i < sc.len; i++) {
			sprite_animate(&sc.sprites[i], delta);
		}

		set_sprite_pos(player, player_pos);

		// generate GPU data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sc.len * sizeof(quad), sc.quads, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// draw
		glClearColor(0.5f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex.id);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.len, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
		frames++;
		if (glfwGetTime() - last_second > 1.0) {
			show_fps(window, frames);
			frames = 0;
			last_second = glfwGetTime();
		}

	}

	glfwTerminate();
	return 0;
}
