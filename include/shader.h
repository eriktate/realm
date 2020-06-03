#ifndef _SHADER_H
#define _SHADER_H

#include "types.h"
#include "gm.h"

u32 load_shader_program(char *vert, char *frag);
void shader_set_vec2(u32 shader_id, char *uniform, vec2 val);
void shader_set_int(u32 shader_id, char *uniform, i32 val);
void shader_set_mat4(u32 shader_id, char *uniform, mat4 val);

#endif // _SHADER_H
