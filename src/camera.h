#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "app.h"

namespace sgltk {

/**
 * @class Camera
 * @brief Manages cameras
 */
class Camera {
public:
	/**
	 * @brief The view matrix
	 */
	glm::mat4 view_matrix;
	/**
	 * @brief The perspective projection matrix
	 */
	glm::mat4 projection_matrix;
	/**
	 * @brief Viewport width
	 */
	float width;
	/**
	 * @brief Viewport height
	 */
	float height;
	/**
	 * @brief The vertical field of view angle of the camera
	 */
	float fovy;
	/**
	 * @brief The far plane of the camera frustum
	 */
	float far_plane;
	/**
	 * @brief The near plane of the camera frustum
	 */
	float near_plane;
	/**
	 * @brief The camera position
	 */
	glm::vec3 position;
	/**
	 * @brief The direction the camera is pointing
	 */
	glm::vec3 direction;
	/**
	 * @brief The up vector of the camera
	 */
	glm::vec3 up;
	/**
	 * @brief The right vector of the camera
	 */
	glm::vec3 right;

	/**
	 * @param position The camera position
	 * @param direction The view direction
	 * @param up The up vector
	 */
	EXPORT Camera(glm::vec3 position,
		      glm::vec3 direction,
		      glm::vec3 up);
	EXPORT ~Camera();

	/**
	 * @brief Recalculates the view matrix
	 */
	EXPORT void update_view_matrix();

	/**
	 * @brief Recalculates the projection matrix
	 */
	EXPORT virtual void update_projection_matrix() = 0;

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
};

/**
 * class P_Camera
 * @brief Manages perspective cameras
 */
class P_Camera : public Camera {
	public:
	EXPORT P_Camera();
	/**
	 * @param camera The camera to copy
	 */
	EXPORT P_Camera(const sgltk::P_Camera& camera);
	/**
	 * @param position The camera position
	 * @param direction The view direction
	 * @param up The up vector
	 */
	EXPORT P_Camera(glm::vec3 position,
			glm::vec3 direction,
			glm::vec3 up);
	/**
	 * @param position The camera position
	 * @param direction The view direction
	 * @param up The up vector
	 * @param fovy The field of view in radians
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 * @param near_plane The near plane
	 * @param far_plane The far plane
	 */
	EXPORT P_Camera(glm::vec3 position,
			glm::vec3 direction,
			glm::vec3 up,
			float fovy,
			float width,
			float height,
			float near_plane,
			float far_plane);
	EXPORT ~P_Camera();
	/**
	 * @brief Recalculates the projection matrix
	 */
	EXPORT void update_projection_matrix();
	/**
	 * @brief Calculates the corner points of the camera's frustum
	 * @return Returns a vector containing the world coordinates of the
	 * 	frustum points.
	 * 	0 - near-bottom-left
	 * 	1 - near-bottom-right
	 * 	2 - near-top-right
	 * 	3 - near-top-left
	 * 	4 - far-bottom-left
	 * 	5 - far-bottom-right
	 * 	6 - far-top-right
	 * 	7 - far-top-left
	 */
	EXPORT std::vector<glm::vec3> calculate_frustum_points();
	/**
	 * @brief Calculates the distances of a point to all planes of the
	 * 	  camera's frustum. Positive values indicate that the point
	 * 	  and the frustum are on the opposite sides of the plane.
	 * @param point The point to calculate the distances to.
	 * @return Returns a vector containing the distances of the point to
	 * 	the planes of the frustum.
	 * 	0 - far
	 * 	1 - near
	 * 	2 - left
	 * 	3 - right
	 * 	4 - top
	 * 	5 - bottom
	 */
	EXPORT std::vector<float> calculate_frustum_distance(glm::vec3 point);
};

/**
 * class O_Camera
 * @brief Manages orthographic cameras
 */
class O_Camera : public Camera {
	public:
	EXPORT O_Camera();
	/**
	 * @param camera The camera to copy
	 */
	EXPORT O_Camera(const O_Camera& camera);
	/**
	 * @param position The camera position
	 * @param direction The view direction
	 * @param up The up vector
	 */
	EXPORT O_Camera(glm::vec3 position,
			glm::vec3 direction,
			glm::vec3 up);
	/**
	 * @param position The camera position
	 * @param direction The view direction
	 * @param up The up vector
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 * @param near_plane The near plane
	 * @param far_plane The far plane
	 */
	EXPORT O_Camera(glm::vec3 position,
			glm::vec3 direction,
			glm::vec3 up,
			float width,
			float height,
			float near_plane,
			float far_plane);
	EXPORT ~O_Camera();
	/**
	 * @brief Recalculates the projection matrix
	 */
	EXPORT void update_projection_matrix();
	/**
	 * @brief Calculates the corner points of the camera's frustum
	 * @return Returns a vector containing the world coordinates of the
	 * 	frustum points.
	 * 	0 - near-bottom-left
	 * 	1 - near-bottom-right
	 * 	2 - near-top-right
	 * 	3 - near-top-left
	 * 	4 - far-bottom-left
	 * 	5 - far-bottom-right
	 * 	6 - far-top-right
	 * 	7 - far-top-left
	 */
	EXPORT std::vector<glm::vec3> calculate_frustum_points();
	/**
	 * @brief Calculates the distances of a point to all planes of the
	 * 	  camera's frustum. Positive values indicate that the point
	 * 	  and the frustum are on the opposite sides of the plane.
	 * @param point The point to calculate the distances to.
	 * @return Returns a vector containing the distances of the point to
	 * 	the planes of the frustum.
	 * 	0 - far
	 * 	1 - near
	 * 	2 - left
	 * 	3 - right
	 * 	4 - top
	 * 	5 - bottom
	 */
	EXPORT std::vector<float> calculate_frustum_distance(glm::vec3 point);
	/**
	 * @brief Sets the camera parameters in such a way as to make the
	 * 	frustum of this camera the bounding box of the frustum of the
	 * 	other camera aligned to the direction vector
	 * @param camera The camera to calculate the bounding box for
	 * @param direction The vector to align the bounding box to
	 * @param offset The offset by which the planes of the frustum will
	 * 	be moved
	 */
	EXPORT void calculate_bounding_frustum(O_Camera& camera, glm::vec3 direction, float offset);
	/**
	 * @brief Sets the camera parameters in such a way as to make the
	 * 	frustum of this camera the bounding box of the frustum of the
	 * 	other camera aligned to the direction vector
	 * @param camera The camera to calculate the bounding box for
	 * @param direction The vector to align the bounding box to
	 * @param offset The offset by which the planes of the frustum will
	 * 	be moved
	 */
	EXPORT void calculate_bounding_frustum(P_Camera& camera, glm::vec3 direction, float offset);
};

/**
 * class IP_Camera
 * @brief Manages infinite perspective cameras
 */
class IP_Camera : public Camera {
	public:
	EXPORT IP_Camera();
	/**
	 * @param camera The camera to copy
	 */
	EXPORT IP_Camera(const IP_Camera& camera);
	/**
	 * @param position The camera position
	 * @param direction The view direction
	 * @param up The up vector
	 */
	EXPORT IP_Camera(glm::vec3 position,
			 glm::vec3 direction,
			 glm::vec3 up);
	/**
	 * @param position The camera position
	 * @param direction The view direction
	 * @param up The up vector
	 * @param fovy The field of view in radians
	 * @param width The width of the viewport
	 * @param height The height of the viewport
	 * @param near_plane The near plane
	 */
	EXPORT IP_Camera(glm::vec3 position,
			 glm::vec3 direction,
			 glm::vec3 up,
			 float fovy,
			 float width,
			 float height,
			 float near_plane);
	EXPORT ~IP_Camera();
	/**
	 * @brief Recalculates the projection matrix
	 */
	EXPORT void update_projection_matrix();
	/**
	 * @brief Calculates the corner points of the camera's frustum
	 * @return Returns a vector containing the world coordinates of the
	 * 	frustum points.
	 * 	0 - near-bottom-left
	 * 	1 - near-bottom-right
	 * 	2 - near-top-right
	 * 	3 - near-top-left
	 */
	EXPORT std::vector<glm::vec3> calculate_frustum_points();
	/**
	 * @brief Calculates the distances of a point to all planes of the
	 * 	  camera's frustum. Positive values indicate that the point
	 * 	  and the frustum are on the opposite sides of the plane.
	 * @param point The point to calculate the distances to.
	 * @return Returns a vector containing the distances of the point to
	 * 	the planes of the frustum.
	 * 	0 - near
	 * 	1 - left
	 * 	2 - right
	 * 	3 - top
	 * 	4 - bottom
	 */
	EXPORT std::vector<float> calculate_frustum_distance(glm::vec3 point);
};

}

#endif
