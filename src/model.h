#ifndef __MODEL_H__
#define __MODEL_H__

#include "app.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include "image.h"
#include "texture.h"

namespace sgltk {

#ifdef HAVE_ASSIMP_H

#ifndef BONES_PER_VERTEX
	#define BONES_PER_VERTEX 4
#endif


/**
 * @class Model
 * @brief Manages imported models
 */
class Model {
	static std::vector<std::string> paths;
	Assimp::Importer importer;
	const aiScene *scene;
	Shader *shader;

	int model_matrix_buf;
	int normal_matrix_buf;

	glm::mat4 *view_matrix;
	glm::mat4 *projection_matrix;

	std::string position_name;
	std::string normal_name;
	std::string tangent_name;
	std::string color_name;
	std::string texture_coordinates_name;
	std::string bone_ids_name;
	std::string bone_weights_name;
	std::string bone_array_name;

	std::vector<glm::mat4> bone_offsets;

	double ticks_per_second;
	glm::mat4 glob_inv_transf;

	void set_vertex_attribute(std::unique_ptr<Mesh>& mesh);
	void traverse_scene_nodes(aiNode *start_node, aiMatrix4x4 *parent_trafo);
	void traverse_animation_nodes(float time, aiNode *node, glm::mat4 parent_transformation);

	std::unique_ptr<Mesh> create_mesh(unsigned int index);
	void compute_bounding_box();

	static aiVector3D interpolate_scaling(float time, aiNodeAnim *node);
	static aiVector3D interpolate_translation(float time, aiNodeAnim *node);
	static aiQuaternion interpolate_rotation(float time, aiNodeAnim *node);
	static glm::mat4 ai_to_glm_mat4(const aiMatrix4x4& in);

	public:
		/**
		 * @brief The model matrix of the model
		 */
		glm::mat4 model_matrix;
		/**
		 * @brief Two corners of the bounding box of the model
		 */
		std::vector<glm::vec3> bounding_box;
		/**
		 * @brief The meshes that make up the model
		 */
		std::vector<std::unique_ptr<Mesh> > meshes;
		/**
		 * @brief The bones of the model's skeleton
		 */
		std::vector<glm::mat4> bones;
		/**
		 * @brief Maps bone names to bone indices
		 * If the bone does not have a name, it will get the name
		 * 'sgltk_bone_i', where 'i' is the index of the bone
		 * in the scene.
		 */
		std::map<std::string, unsigned int> bone_map;
		/**
		 * @brief Maps mesh names to mesh indices
		 * If the mesh does not have a name, it will get the name
		 * 'sgltk_mesh_i', where 'i' is the index of the mesh
		 * in the scene.
		 */
		std::map<std::string, unsigned int> mesh_map;

		EXPORT Model();
		EXPORT ~Model();

		/**
		 * @brief Adds a new path where files will be loaded from
		 * @param path A new path to use
		 * @note To avoid duplicates this function first performs
		 *	 a search on existing entries.
		 */
		EXPORT static void add_path(std::string path);

		/**
		 * @brief Loads a model from file
		 * @param filename The file to be loaded
		 * @return Returns true on success, false otherwise
		 * @note If the path you pass to this function is not an
		 *	 absolute path, all directories you specified using the
		 *	 add_path function will be searched in addition to the
		 *	 current working directory.
		 */
		EXPORT bool load(const std::string& filename);
		/**
		 * @brief Specifies the shader to use to render the mesh
		 * @param shader The shader to be used to render the mesh
		 */
		EXPORT void setup_shader(Shader *shader);
		/**
		 * @brief Sets up the view and projection matrices that will be
		 *	  used by the meshes in the model
		 * @param view_matrix The view matrix
		 * @param projection_matrix The projection matrix
		 * @return Returns true if both pointers are not NULL pointers, flase otherwise
		 */
		EXPORT bool setup_camera(glm::mat4 *view_matrix,
					 glm::mat4 *projection_matrix);
		/**
		 * @brief Sets up the view and projection matrices that will be
		 *	  used by the meshes in the model
		 * @param camera The camera to use
		 * @return Returns true on success, false otherwise
		 */
		EXPORT bool setup_camera(Camera *camera);
		/**
		 * @brief Sets the position vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "pos_in"
		 */
		EXPORT void set_position_name(const std::string& name);
		/**
		 * @brief Sets the normal vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "norm_in"
		 */
		EXPORT void set_normal_name(const std::string& name);
		/**
		 * @brief Sets the tangent vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "tang_in"
		 */
		EXPORT void set_tangent_name(const std::string& name);
		/**
		 * @brief Sets the color vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "col_in"
		 */
		EXPORT void set_color_name(const std::string& name);
		/**
		 * @brief Sets the texture coordinates vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "tex_coord_in"
		 */
		EXPORT void set_texture_coordinates_name(const std::string& name);
		/**
		 * @brief Sets the bone ids vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "bone_ids_in"
		 */
		EXPORT void set_bone_ids_name(const std::string& name);
		/**
		 * @brief Sets the bone weights vertex attribute name in the shader
		 * @param name The new vertex attribute name. An empty string resets
		 *		the name to the default value "bone_weights_in"
		 */
		EXPORT void set_bone_weights_name(const std::string& name);
		/**
		* @brief Sets the bone array uniform name in the shader
		* @param name The new uniform name. An empty string resets
		*		the name to the default value "bone_array"
		*/
		EXPORT void set_bone_array_name(const std::string& name);
		/**
		 * @brief Sets the animation speed.
		 * @param speed The speed multiplier
		 */
		EXPORT void set_animation_speed(double speed);
		/**
		 * @brief Attaches a texture to every mesh of the model
		 * @param name The name of the texture in the shader
		 * @param texture The texture to be bound when rendering the model
		 * @param index The index of the texture in the uniform array
		 */
		EXPORT void attach_texture(const std::string& name,
					   const Texture& texture,
					   unsigned int index = 0);
		/**
		 * @brief Sets a texture parameter for all attached textures
		 * 	except for manually attached textures.
		 * @param name The name of the texture parameter
		 * @param parameter The parameter value
		 */
		EXPORT void set_texture_parameter(GLenum name, int parameter);
		/**
		 * @brief Sets a texture parameter for all attached textures
		 * 	except for manually attached textures.
		 * @param name The name of the texture parameter
		 * @param parameter The parameter value
		 */
		EXPORT void set_texture_parameter(GLenum name, float parameter);
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
		 * @brief Sets the model and normal matrix vertex attributes
		 * This function is meant to be used to update the attribute locations
		 * after a shader swap.
		 */
		EXPORT void set_instanced_matrix_attributes(void);
		/**
		 * @brief Draws all associated meshes with the index buffer 0.
		 * @param model_matrix The model matrix to use
		 *	  (NULL to use the model_matrix member)
		 */
		EXPORT void draw(const glm::mat4 *model_matrix = NULL);
		/**
		 * @brief Draws all associated meshes multiple times
		 * @param num_instances The number of instances to be drawn
		 */
		EXPORT void draw_instanced(unsigned int num_instances);
	};

#endif //HAVE_ASSIMP_H

}

#endif //__MODEL_H__
