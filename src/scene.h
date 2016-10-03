#ifndef __SCENE_H__
#define __SCENE_H__

#include "app.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include "image.h"
#include "texture.h"

#ifndef BONES_PER_VERTEX
	#define BONES_PER_VERTEX 4
#endif

namespace sgltk {

/**
 * @class Scene
 * @brief Manages imported scenes
 */
class Scene {
	typedef struct Bone {
		aiMatrix4x4 transformation;
		aiMatrix4x4 offset;
	} Bone;

	static std::vector<std::string> paths;
	Assimp::Importer importer;
	const aiScene *scene;
	Shader *shader;

	std::string position_name;
	std::string normal_name;
	std::string tangent_name;
	std::string color_name;
	std::string texture_coordinates_name;
	std::string bone_ids_name;
	std::string bone_weights_name;
	std::string bone_array_name;

	double ticks_per_second;
	std::vector<Bone> bones;
	aiMatrix4x4 glob_inv_transf;
	std::map<std::string, unsigned int> bone_map;

	std::map<std::string, unsigned int> mesh_map;

	void set_vertex_attribute(Mesh *mesh);
	void traverse_scene_nodes(aiNode *start_node, aiMatrix4x4 *parent_trafo);
	void traverse_animation_nodes(float time, aiNode *node, aiMatrix4x4 parent_transformation);

	Mesh *create_mesh(unsigned int index);
	void compute_bounding_box();

	static aiVector3D interpolate_scaling(float time, aiNodeAnim *node);
	static aiVector3D interpolate_translation(float time, aiNodeAnim *node);
	static aiQuaternion interpolate_rotation(float time, aiNodeAnim *node);
	static glm::mat4 ai_to_glm_mat4(const aiMatrix4x4& in);

	public:
		glm::mat4 model_matrix;
		std::vector<glm::vec3> bounding_box;
		std::vector<Mesh *> meshes;

		EXPORT Scene();
		EXPORT ~Scene();

		/**
		 * @brief Adds a new path where files will be loaded from
		 * @param path A new path to use
		 */
		EXPORT static void add_path(std::string path);

		/**
		 * @brief Loads a scene from file
		 * @param filename The file to be loaded
		 * @return Returns true on success, false otherwise
		 * @note If the path you pass to this function is not an
		 *	 absolute path, all directories you specified using the
		 *	 add_path function will be searched in addition to the
		 *	 current working directory.
		 */
		EXPORT bool load(std::string filename);
		/**
		 * @brief Specifies the shader to use to render the mesh
		 * @param shader The shader to be used to render the mesh
		 */
		EXPORT void setup_shader(Shader *shader);
		/**
		 * @brief Sets up the view and projection matrices that will be
		 *	  used by the meshes in the scene
		 * @param view_matrix The view matrix
		 * @param projection_matrix The projection matrix
		 * @return Returns true if both pointers are not NULL pointers, flase otherwise
		 */
		EXPORT bool setup_camera(glm::mat4 *view_matrix,
					 glm::mat4 *projection_matrix);
		/**
		 * @brief Sets up the view and projection matrices that will be
		 *	  used by the meshes in the scene
		 * @param camera The camera to use
		 * @param type The type of projection to use if the camera has more than one type
		 * @return Returns true on success, false otherwise
		 */
		EXPORT bool setup_camera(Camera *camera, sgltk::CAMERA_TYPE type = sgltk::PERSPECTIVE);
		/**
		 * @brief Sets the position vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "pos_in"
		 */
		EXPORT void set_position_name(std::string name);
		/**
		 * @brief Sets the normal vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "norm_in"
		 */
		EXPORT void set_normal_name(std::string name);
		/**
		 * @brief Sets the tangent vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "tang_in"
		 */
		EXPORT void set_tangent_name(std::string name);
		/**
		 * @brief Sets the color vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "col_in"
		 */
		EXPORT void set_color_name(std::string name);
		/**
		 * @brief Sets the texture coordinates vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "tex_coord_in"
		 */
		EXPORT void set_texture_coordinates_name(std::string name);
		/**
		 * @brief Sets the bone ids vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "bone_ids_in"
		 */
		EXPORT void set_bone_ids_name(std::string name);
		/**
		 * @brief Sets the bone weights vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "bone_weights_in"
		 */
		EXPORT void set_bone_weights_name(std::string name);
		/**
		* @brief Sets the bone array uniform name in the shader
		* @param name The new uniform name. An empty string resets
		*		the name to the default value "bone_array"
		*/
		EXPORT void set_bone_array_name(std::string name);
		/**
		 * @brief Sets the animation speed.
		 * @param speed The speed multiplier
		 */
		EXPORT void set_animation_speed(double speed);
		/**
		 * @brief Calculates a new bone pose based on the animation time
		 * @param time The current animation time. If time is greater than
		 * 	the duration of the animation 
		 * @return Returns true on success, false otherwise
		 */
		EXPORT bool animate(float time);
		/**
		 * @brief Attaches the buffers and sets the model and normal
		 * 		matrix vertex attributes
		 * @param model_matrix The model matrices to be used for
		 * 		instanced drawing
		 * @param usage A hint as to how the buffer will be accessed.
		 * 	Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
		 */
		EXPORT void setup_instanced_matrix(const std::vector<glm::mat4>& model_matrix,
							GLenum usage = GL_STATIC_DRAW);
		/**
		 * @brief Draws all associated meshes with the index buffer 0.
		 */
		EXPORT void draw();
		/**
		 * @brief Draws all associated meshes with the index buffer 0.
		 * @param model_matrix The model matrix to use
		 *	  (NULL to use the model_matrix member)
		 */
		EXPORT void draw(const glm::mat4 *model_matrix);
		/**
		 * @brief Draws all associated meshes multiple times
		 * @param num_instances The number of instances to be drawn
		 */
		EXPORT void draw_instanced(unsigned int num_instances);
	};

}

#endif
