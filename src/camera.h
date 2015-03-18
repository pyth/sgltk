#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core.h"

class Camera {
	glm::mat4 ident;
public:
	glm::vec4 pos;
	glm::vec4 forward;
	glm::vec4 up;
	glm::vec4 right;

	Camera();
	~Camera();

	void move_up(float delta);
	void move_right(float delta);
	void move_forward(float delta);

	void yaw(float angle);
	void roll(float angle);
	void pitch(float angle);
};

#endif
