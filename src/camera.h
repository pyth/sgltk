#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core.h"

/**
 * @class Camera
 * @brief Manages cameras
 */
class Camera {
	glm::mat4 ident;
public:
	/**
	 * @brief The camera position
	 */
	glm::vec4 pos;
	/**
	 * @brief The direction the camera is pointing
	 */
	glm::vec4 dir;
	/**
	 * @brief The up vector of the camera
	 */
	glm::vec4 up;
	/**
	 * @brief The right vector of the camera
	 */
	glm::vec4 right;

	Camera();
	/**
	 * @param pos The camera position
	 * @param dir The view direction
	 * @param up The up vector
	 */
	Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up);
	~Camera();

	/**
	 * @brief Move the camera along the up vector
	 * @param delta The distance to move the camera
	 */
	void move_up(float delta);
	/**
	 * @brief Move the camera along the right vector
	 * @param delta The distance to move the camera
	 */
	void move_right(float delta);
	/**
	 * @brief Move the camera along the forward vector
	 * @param delta The distance to move the camera
	 */
	void move_forward(float delta);

	/**
	 * @brief Rotate the camera using the up vector as the axis
	 * @param angle The angle to rotate the camera
	 */
	void yaw(float angle);
	/**
	 * @brief Rotate the camera using the forward vector as the axis
	 * @param angle The angle to rotate the camera
	 */
	void roll(float angle);
	/**
	 * @brief Rotate the camera using the right vector as the axis
	 * @param angle The angle to rotate the camera
	 */
	void pitch(float angle);
};

#endif
