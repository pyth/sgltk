#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core.h"

namespace sgltk {
	enum CAMERA_TYPE {
		PERSPECTIVE =	1 << 0,
		ORTHOGRAPHIC =	1 << 1,
		BOTH =		PERSPECTIVE | ORTHOGRAPHIC
	};
}

/**
 * @class Camera
 * @brief Manages cameras
 * A new camera uses perspective projection matrix if nothing else is specified
 */
class Camera {
	glm::mat4 ident;
public:
	/**
	 * @brief The camera type
	 * The type of the projection matrix used by the camera.
	 * It can be a bitwise or combination of the types. 
	 */
	sgltk::CAMERA_TYPE type;
	/**
	 * @brief The view matrix
	 */
	glm::mat4 view_matrix;
	/**
	 * @brief The perspective projection matrix
	 */
	glm::mat4 projection_matrix_persp;
	/**
	 * @brief The orthographic projection matrix
	 */
	glm::mat4 projection_matrix_ortho;
	/**
	 * @brief The field of view
	 */
	float fov;
	/**
	 * @brief The near plane
	 */
	float near_plane;
	/**
	 * @brief The far plane
	 */
	float far_plane;
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
	 * @param type The camera type
	 */
	Camera(sgltk::CAMERA_TYPE type);
	/**
	 * @param pos The camera position
	 * @param dir The view direction
	 * @param up The up vector
	 */
	Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up);
	/**
	 * @param pos The camera position
	 * @param dir The view direction
	 * @param up The up vector
	 * @param type The camera type
	 */
	Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, sgltk::CAMERA_TYPE type);
	/**
	 * @param pos The camera position
	 * @param dir The view direction
	 * @param up The up vector
	 * @param fov The field of view
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 * @param near_plane The near plane
	 * @param far_plane The far plane
	 */
	Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up,
	       float fov, float width, float height,
	       float near_plane, float far_plane);
	/**
	 * @param pos The camera position
	 * @param dir The view direction
	 * @param up The up vector
	 * @param fov The field of view
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 * @param near_plane The near plane
	 * @param far_plane The far plane
	 * @param type The camera type
	 */
	Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up,
	       float fov, float width, float height,
	       float near_plane, float far_plane,
	       sgltk::CAMERA_TYPE type);
	~Camera();

	/**
	 * @brief Recalculates the view matrix
	 */
	void update_view_matrix();

	/**
	 * @brief Recalculates the projection matrix
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 */
	void update_projection_matrix(float width, float height);
	/**
	 * @brief Recalculates the projection matrix
	 * @param fov The field of view
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 */
	void update_projection_matrix(float fov, float width, float height);
	/**
	 * @brief Recalculates the projection matrix
	 * @param fov The field of view
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 * @param near_plane The near plane
	 * @param far_plane The far plane
	 */
	void update_projection_matrix(float fov, float width, float height,
				      float near_plane, float far_plane);

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
