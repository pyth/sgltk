#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "app.h"

namespace sgltk {
	enum EXPORT CAMERA_TYPE {
		PERSPECTIVE =		1 << 0,
		INF_PERSPECTIVE =	1 << 1,
		ORTHOGRAPHIC =		1 << 2,
	};

/**
 * @class Camera
 * @brief Manages cameras
 * A new camera uses perspective projection matrix if nothing else is specified
 */
class Camera {
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
	 * @brief The infinite projection matrix
	 */
	glm::mat4 projection_matrix_persp_inf;
	/**
	 * @brief The orthographic projection matrix
	 */
	glm::mat4 projection_matrix_ortho;
	/**
	 * @brief The field of view
	 */
	float fovy;
	/**
	 * @brief Viewport width
	 */
	float width;
	/**
	 * @brief Viewport height
	 */
	float height;
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

	EXPORT Camera();
	/**
	 * @param pos The camera position
	 * @param dir The view direction
	 * @param up The up vector
	 * @param type The camera type
	 */
	EXPORT Camera(glm::vec3 pos,
		      glm::vec3 dir,
		      glm::vec3 up,
		      sgltk::CAMERA_TYPE type = sgltk::PERSPECTIVE);
	/**
	 * @param pos The camera position
	 * @param dir The view direction
	 * @param up The up vector
	 * @param fovy The field of view
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 * @param near_plane The near plane
	 * @param far_plane The far plane
	 * @param type The camera type
	 */
	EXPORT Camera(glm::vec3 pos,
		      glm::vec3 dir,
		      glm::vec3 up,
		      float fovy,
		      float width,
		      float height,
		      float near_plane,
		      float far_plane,
		      sgltk::CAMERA_TYPE type = sgltk::PERSPECTIVE);
	EXPORT ~Camera();

	/**
	 * @brief Recalculates the view matrix
	 */
	EXPORT void update_view_matrix();

	/**
	 * @brief Recalculates the projection matrix
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 */
	EXPORT void update_projection_matrix(float width, float height);
	/**
	 * @brief Recalculates the projection matrix
	 * @param fovy The field of view
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 */
	EXPORT void update_projection_matrix(float fovy, float width, float height);
	/**
	 * @brief Recalculates the projection matrix
	 * @param fovy The field of view
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 * @param near_plane The near plane
	 */
	EXPORT void update_projection_matrix(float fovy, float width, float height,
				      float near_plane);
	/**
	 * @brief Recalculates the projection matrix
	 * @param fovy The field of view
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 * @param near_plane The near plane
	 * @param far_plane The far plane
	 */
	EXPORT void update_projection_matrix(float fovy, float width, float height,
				      float near_plane, float far_plane);

	/**
	 * @brief Move the camera along the up vector
	 * @param delta The distance to move the camera
	 */
	EXPORT void move_up(float delta);
	/**
	 * @brief Move the camera along the right vector
	 * @param delta The distance to move the camera
	 */
	EXPORT void move_right(float delta);
	/**
	 * @brief Move the camera along the forward vector
	 * @param delta The distance to move the camera
	 */
	EXPORT void move_forward(float delta);
	/**
	 * @brief Move the camera by the vector (x, y, z)
	 * @param x The x component
	 * @param y The y component
	 * @param z The z component
	 */
	EXPORT void move_by(float x, float y,  float z);
	/**
	 * @brief Move the camera by the vector
	 * @param vector The vector to move the camera by
	 */
	EXPORT void move_by(glm::vec3 vector);
	/**
	 * @brief Rotate the camera using the up vector as the axis
	 * @param angle The angle to rotate the camera
	 */
	EXPORT void yaw(float angle);
	/**
	 * @brief Rotate the camera using the forward vector as the axis
	 * @param angle The angle to rotate the camera
	 */
	EXPORT void roll(float angle);
	/**
	 * @brief Rotate the camera using the right vector as the axis
	 * @param angle The angle to rotate the camera
	 */
	EXPORT void pitch(float angle);
	/**
	 * @brief Calculates the corner points of the camera's frustum
	 * @param near_bottom_left The world coordinates of the near-bottom-left
	 * 		point of the frustum. Can be NULL.
	 * @param near_bottom_right The world coordinates of the near-bottom-right
	 * 		point of the frustum. Can be NULL.
	 * @param near_top_right The world coordinates of the near-top-right
	 * 		point of the frustum. Can be NULL.
	 * @param near_top_left The world coordinates of the near-top-left
	 * 		point of the frustum. Can be NULL.
	 * @param far_bottom_left The world coordinates of the far-bottom-left
	 * 		point of the frustum. Can be NULL.
	 * @param far_bottom_right The world coordinates of the far-bottom-right
	 * 		point of the frustum. Can be NULL.
	 * @param far_top_right The world coordinates of the far-bottom-right
	 * 		point of the frustum. Can be NULL.
	 * @param far_top_left The world coordinates of the far-bottom-left
	 * 		point of the frustum. Can be NULL.
	 * @return Returns true on success, false otherwise.
	 */
	EXPORT bool calculate_frustum_points(glm::vec3 *near_bottom_left,
					     glm::vec3 *near_bottom_right,
					     glm::vec3 *near_top_right,
					     glm::vec3 *near_top_left,
					     glm::vec3 *far_bottom_left,
					     glm::vec3 *far_bottom_right,
					     glm::vec3 *far_top_right,
					     glm::vec3 *far_top_left);
	/**
	 * @brief Calculates the corner points of the camera's frustum
	 * @param near_bottom_left The world coordinates of the near-bottom-left
	 * 		point of the frustum. Can be NULL.
	 * @param near_bottom_right The world coordinates of the near-bottom-right
	 * 		point of the frustum. Can be NULL.
	 * @param near_top_right The world coordinates of the near-top-right
	 * 		point of the frustum. Can be NULL.
	 * @param near_top_left The world coordinates of the near-top-left
	 * 		point of the frustum. Can be NULL.
	 * @param far_bottom_left The world coordinates of the far-bottom-left
	 * 		point of the frustum. Can be NULL.
	 * @param far_bottom_right The world coordinates of the far-bottom-right
	 * 		point of the frustum. Can be NULL.
	 * @param far_top_right The world coordinates of the far-bottom-right
	 * 		point of the frustum. Can be NULL.
	 * @param far_top_left The world coordinates of the far-bottom-left
	 * 		point of the frustum. Can be NULL.
	 * @param type The type of the projection matrix.
	 * @return Returns true on success, false otherwise.
	 */
	EXPORT bool calculate_frustum_points(glm::vec3 *near_bottom_left,
					     glm::vec3 *near_bottom_right,
					     glm::vec3 *near_top_right,
					     glm::vec3 *near_top_left,
					     glm::vec3 *far_bottom_left,
					     glm::vec3 *far_bottom_right,
					     glm::vec3 *far_top_right,
					     glm::vec3 *far_top_left,
					     sgltk::CAMERA_TYPE type);
	/**
	 * @brief Calculates the distances of a point to all planes of the
	 * 	  camera's frustum. Positive values indicate that the point
	 * 	  and the frustum are on the opposite sides of the plane.
	 * @param position The position of the point to calculate the distance to
	 * @param far The distance to the far plane of the frustum. Can be NULL.
	 * @param near The distance to the near plane of the frustum. Can be NULL.
	 * @param left The distance to the left plane of the frustum. Can be NULL.
	 * @param right The distance to the right plane of the frustum. Can be NULL.
	 * @param top The distance to the top plane of the frustum. Can be NULL.
	 * @param bottom The distance to the bottom plane of the frustum. Can be NULL.
	 * @return Returns true on success, false otherwise.
	 */
	EXPORT bool calculate_frustum_distance(glm::vec3 position,
					       float *far,
					       float *near,
					       float *left,
					       float *right,
					       float *top,
					       float *bottom);
	/**
	 * @brief Calculates the distances of a point to all planes of the
	 * 	  camera's frustum. Positive values indicate that the point
	 * 	  and the frustum are on the opposite sides of the plane.
	 * @param position The position of the point to calculate the distance to
	 * @param far The distance to the far plane of the frustum. Can be NULL.
	 * @param near The distance to the near plane of the frustum. Can be NULL.
	 * @param left The distance to the left plane of the frustum. Can be NULL.
	 * @param right The distance to the right plane of the frustum. Can be NULL.
	 * @param top The distance to the top plane of the frustum. Can be NULL.
	 * @param bottom The distance to the bottom plane of the frustum. Can be NULL.
	 * @param type The type of the projection matrix.
	 * @return Returns true on success, false otherwise.
	 */
	EXPORT bool calculate_frustum_distance(glm::vec3 position,
					       float *far,
					       float *near,
					       float *left,
					       float *right,
					       float *top,
					       float *bottom,
					       sgltk::CAMERA_TYPE type);
};
}

#endif
