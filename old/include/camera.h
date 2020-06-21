#ifndef _CAMERA_H
#define _CAMERA_H

#include "types.h"
#include "gm.h"

typedef struct camera {
	vec3 pos;
	int width;
	int height;
	int inner_width;
	int inner_height;
} camera;

camera new_camera(vec3 pos, int width, int height, int inner_width, int inner_height);
void look_at(camera *c, vec3 pos);
mat4 get_camera_transform(camera *c);


#endif // _CAMERA_H
