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
#include "gl.h"
#include "input.h"
#include "camera.h"

const i32 WIDTH = 1280;
const i32 HEIGHT = 960;
bool debug = true;
vec3 player_pos;

void process_input(GLFWwindow *w, controller *ctrl) {
	clear_inputs(ctrl);

	if (glfwGetKey(w, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(w, true);
	}

	if (glfwGetKey(w, GLFW_KEY_SEMICOLON)) {
		set_input(ctrl, INPUT_TYPE_DEBUG, 1);
	}

	if (glfwGetKey(w, GLFW_KEY_D)) {
		set_input(ctrl, INPUT_TYPE_RIGHT, 1);
	}

	if (glfwGetKey(w, GLFW_KEY_A)) {
		set_input(ctrl, INPUT_TYPE_LEFT, 1);
	}

	if (glfwGetKey(w, GLFW_KEY_W)) {
		set_input(ctrl, INPUT_TYPE_UP, 1);
	}

	if (glfwGetKey(w, GLFW_KEY_S)) {
		set_input(ctrl, INPUT_TYPE_DOWN, 1);
	}

	f64 mouse_x, mouse_y;
	glfwGetCursorPos(w, &mouse_x, &mouse_y);
	ctrl->cursor_pos = new_vec2((f32)mouse_x, (f32)mouse_y);

	finalize_inputs(ctrl);
}

// brute force indices for a number of quads
elements generate_quad_indices(i32 quad_count) {
	size_t len = quad_count * 6;
	size_t size = len * sizeof(u32);

	u32 *indices = (u32 *)malloc(size);
	for (i32 i = 0; i < quad_count; i++) {
		u32 tl = i * 4;
		u32 tr = i * 4 + 1;
		u32 bl = i * 4 + 2;
		u32 br = i * 4 + 3;

		indices[i*6] = tl;
		indices[i*6+1] = tr;
		indices[i*6+2] = bl;
		indices[i*6+3] = tr;
		indices[i*6+4] = bl;
		indices[i*6+5] = br;
	}

	return (elements){
		len,
		size,
		indices,
	};
}

// brute force indices for a number of rectangles
elements generate_rect_indices(i32 rect_count) {
	size_t len = rect_count * 8;
	size_t size = len * sizeof(u32);

	u32 *indices = (u32 *)malloc(size);
	for (i32 i = 0; i < rect_count; i++) {
		u32 tl = i * 4;
		u32 tr = i * 4 + 1;
		u32 br = i * 4 + 2;
		u32 bl = i * 4 + 3;

		indices[i*8] = tl;
		indices[i*8+1] = tr;
		indices[i*8+2] = tr;
		indices[i*8+3] = br;
		indices[i*8+4] = br;
		indices[i*8+5] = bl;
		indices[i*8+6] = bl;
		indices[i*8+7] = tl;
	}

	return (elements){
		len,
		size,
		indices,
	};
}

void show_fps(GLFWwindow *w, i32 fps) {
	char title[128];

	sprintf(title, "Playground: %d", fps);
	glfwSetWindowTitle(w, title);
}

i32 random_i32(f64 seed, i32 lower, i32 upper) {
	srand((u32)seed);
	return lower + (rand() % (upper - lower));
}

int main(void)
{
	GLFWwindow* window = create_window(WIDTH, HEIGHT, "float - playground");

	// load texture
	texture tex;
	if (load_texture("spritesheet.png", &tex)) {
		printf("failed to load texture\n");
		return -1;
	}

	scene sc = create_scene(5000);

	atlas sprite_atlas = new_atlas(tex, 128, 128, 16, 32, 0, 0, 3, 8);
	atlas tile_atlas = new_atlas(tex, 0, 0, 16, 16, 0, 0, 12, 7);

	for (i32 i = 0; i < 32; i++) {
		for (i32 j = 0; j < 32; j++) {
			sprite tile = new_sprite(
				new_vec3(256 + (j * 16), 128 + (i * 16), 0),
				16,
				16,
				false
			);
			i32 frame_idx = random_i32(glfwGetTime() + (i * j), 29, 31);
			tile.tex = tile_atlas.frames[frame_idx];
			scene_add_sprite(&sc, tile);
		}
	}

	player_pos = new_vec3(320.0f, 240.0f, 0.0f);
	sprite player_meta = new_sprite(player_pos, 16, 32, true);
	sprite lady_meta = new_sprite(new_vec3(256.0f, 128.0f, 0.0f), 16, 32, true);

	int player_frames[8] = {8, 9, 10, 11, 12, 13, 14, 15};
	int lady_frames[8] = {0, 1, 2, 3, 4, 5, 6, 7};

	player_meta.anim = new_animation(sprite_atlas, 6, 3, player_frames);
	lady_meta.anim = new_animation(sprite_atlas, 6, 3, lady_frames);

	sprite *player = scene_add_sprite(&sc, player_meta);
	sprite *lady = scene_add_sprite(&sc, lady_meta);
	lady->hitbox = new_rect(0, 16, lady->width, lady->height - 16);
	player->hitbox = new_rect(0, 16, player->width, player->height - 16);

	elements quad_indices = generate_quad_indices(sc.len);
	elements rect_indices = generate_rect_indices(2);

	// prep for sprite rendering
	u32 sprites_vao = create_vao();
	u32 sprites_vbo = create_scene_vbo(sprites_vao, sc);
	u32 sprites_ebo = create_ebo(sprites_vao, quad_indices, GL_DYNAMIC_DRAW);

	rect hb = get_hitbox(player);
	// prep for geometry rendering
	vec3 hb_data[8] = {
		new_vec3(hb.x, hb.y, 0),
		new_vec3(hb.x + hb.width, hb.y, 0),
		new_vec3(hb.x + hb.width, hb.y + hb.height, 0),
		new_vec3(hb.x, hb.y + hb.height, 0)
	};

	u32 geo_vao = create_vao();
	u32 geo_vbo = create_vbo(geo_vao, sizeof(vec3) * 4, hb_data, GL_DYNAMIC_DRAW);
	u32 geo_ebo = create_ebo(geo_vao, rect_indices, GL_DYNAMIC_DRAW);
	glBindVertexArray(geo_vao);
	glBindBuffer(GL_ARRAY_BUFFER, geo_vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// load shaders
	u32 sprite_shader = load_shader_program("shaders/sprite_vs.glsl", "shaders/sprite_fs.glsl");
	shader_set_vec2(sprite_shader, "window_size", new_vec2(WIDTH, HEIGHT));
	shader_set_int(sprite_shader, "tex", GL_TEXTURE0);

	u32 geo_shader = load_shader_program("shaders/geo_vs.glsl", "shaders/geo_fs.glsl");
	shader_set_vec2(geo_shader, "window_size", new_vec2(WIDTH, HEIGHT));

	// alpha in texture won't work without setting the blend mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	f64 last_frame = glfwGetTime();
	f64 last_second = glfwGetTime();
	i32 frames = 0;
	f64 delta = 0;
	controller player_ctrl = new_controller();


	camera cam = new_camera(player_pos, WIDTH/2, HEIGHT/2, 128, 128);
	while (!glfwWindowShouldClose(window))
	{
		// update
		delta = glfwGetTime() - last_frame;
		last_frame = glfwGetTime();
		process_input(window, &player_ctrl);
		f32 move_x = get_input(&player_ctrl, INPUT_TYPE_RIGHT) - get_input(&player_ctrl, INPUT_TYPE_LEFT);
		f32 move_y = get_input(&player_ctrl, INPUT_TYPE_DOWN) - get_input(&player_ctrl, INPUT_TYPE_UP);

		vec3 move_vec = new_vec3(move_x, move_y, 0);

		// normalize and scale to move speed
		move_vec = scale3(unit3(move_vec), 2.0f);
		vec3 new_vec = add_vec3(player_pos, move_vec);

		rect player_rect = get_potential_hitbox(player, new_vec);
		rect lady_rect = get_hitbox(lady);
		if (overlaps(player_rect, lady_rect)) {
			vec3 initial_vec = new_vec;
			new_vec.x = player_pos.x;
			player_rect = get_potential_hitbox(player, new_vec);
			if (overlaps(player_rect, lady_rect)) {
				new_vec = initial_vec;
				new_vec.y = player_pos.y;
				player_rect = get_potential_hitbox(player, new_vec);
				if (overlaps(player_rect, lady_rect)) {
					new_vec.x = 0;
					new_vec.y = 0;
				}
			}
		}

		player_pos = new_vec;

		// get updated camera stuff
		look_at(&cam, player_pos);
		mat4 transform = get_camera_transform(&cam);

		shader_set_mat4(sprite_shader, "transform", transform);
		shader_set_mat4(geo_shader, "transform", transform);

		// animate
		for (i32 i = 0; i < sc.len; i++) {
			sprite_animate(&sc.sprites[i], delta);
		}

		set_sprite_pos(player, player_pos);

		// generate GPU data
		update_scene_vbo(sprites_vao, sprites_vbo, sc);
		rect hb = get_hitbox(player);
		// player hb
		hb_data[0] = new_vec3(hb.x, hb.y, 0);
		hb_data[1] = new_vec3(hb.x + hb.width, hb.y, 0);
		hb_data[2] = new_vec3(hb.x + hb.width, hb.y + hb.height, 0);
		hb_data[3] = new_vec3(hb.x, hb.y + hb.height, 0);

		// mouse hb
		i32 cursor_x = ((i32)(player_ctrl.cursor_pos.x / 16)) * 16;
		i32 cursor_y = ((i32)(player_ctrl.cursor_pos.y / 16)) * 16;

		hb_data[4] = new_vec3(cursor_x, cursor_y, 0);
		hb_data[5] = new_vec3(cursor_x + 16, cursor_y, 0);
		hb_data[6] = new_vec3(cursor_x + 16, cursor_y + 16, 0);
		hb_data[7] = new_vec3(cursor_x, cursor_y + 16, 0);

		update_vbo(geo_vao, geo_vbo, sizeof(vec3) * 8, hb_data, GL_DYNAMIC_DRAW);

		// draw
		glClearColor(0.5f, 0.8f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw sprites
		glUseProgram(sprite_shader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex.id);
		glBindVertexArray(sprites_vao);
		glDrawElements(GL_TRIANGLES, quad_indices.len, GL_UNSIGNED_INT, 0);

		if (get_input_pressed(&player_ctrl, INPUT_TYPE_DEBUG)) {
			debug = !debug;
		}

		// draw geometry
		if (debug) {
			glUseProgram(geo_shader);
			glBindVertexArray(geo_vao);
			glDrawElements(GL_LINES, rect_indices.len, GL_UNSIGNED_INT, 0);
		}

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
