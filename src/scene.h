#ifndef __SCENE_H__
#define __SCENE_H__

#include "core.h"
#include "mesh.h"
#include "shader.h"
#include "image.h"
#include "texture.h"

#ifndef BONES_PER_VERTEX
	#define BONES_PER_VERTEX 4
#endif

namespace sgltk {

typedef struct Bone {
	aiMatrix4x4 transformation;
	aiMatrix4x4 offset;
} Bone;

/**
 * @class Scene
 * @brief Manages imported scenes
 */
class Scene {
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
	
	glm::mat4 *view_matrix;
	glm::mat4 *projection_matrix;

	double ticks_per_second;
	std::vector<Bone> bones;
	aiMatrix4x4 glob_inv_transf;
	std::map<std::string, unsigned int> bone_map;

	std::map<std::string, unsigned int> mesh_map;

	void traverse_scene_nodes(aiNode *start_node, aiMatrix4x4 *parent_trafo);
	void traverse_animation_nodes(float time, aiNode *node, aiMatrix4x4 parent_transformation);
	Mesh *create_mesh(aiMesh *mesh);
	void compute_bounding_box();
	static aiVector3D interpolate_scaling(float time, aiNodeAnim *node);
	static aiVector3D interpolate_translation(float time, aiNodeAnim *node);
	static aiQuaternion interpolate_rotation(float time, aiNodeAnim *node);
	static glm::mat4 ai_to_glm_mat4(aiMatrix4x4 *in);
	public:
		glm::mat4 model_matrix;
		std::vector<glm::vec3> bounding_box;
		std::vector<Mesh *> meshes;

		Scene();
		~Scene();

		/**
		 * @brief Adds a new path where files will be loaded from
		 * @param path A new path to use
		 */
		static void add_path(std::string path);

		/**
		 * @brief Loads a scene from file
		 * @param filename The file to be loaded
		 * @return Returns true on success, false otherwise
		 * @note If the path you pass to this function is not an
		 *	 absolute path, all directories you specified using the
		 *	 add_path function will be searched in addition to the
		 *	 current working directory.
		 */
		bool load(std::string filename);
		/**
		 * @brief Specifies the shader to use to render the mesh
		 * @param shader The shader to be used to render the mesh
		 */
		void setup_shader(Shader *shader);
		/**
		 * @brief Sets up the view and projection matrices that will be
		 *	  used by the meshes in the scene
		 * @param view_matrix The view matrix
		 * @param projection_matrix The projection matrix
		 */
		void setup_camera(glm::mat4 *view_matrix,
				  glm::mat4 *projection_matrix);
		/**
		 * @brief Sets the names of the vertex attribute variables
		 *		in the shader. Pass a NULL for a name to keep
		 *		it's value!
		 * @param position_name The name of the position vector variable
		 * @param normal_name The name of the normal vector variable
		 * @param tangent_name The name of the tangent vector variable
		 * @param color_name The name of the color vector variable
		 * @param texture_coordinates_name The name of the texture coordinates
		 *			variable
		 * @param bone_ids_name The name of the bone id array
		 * @param bone_weights_name The name of the bone weight array
		 * @note A number starting at 0 will be appended to the
		 *	 color_name and texture_coordinates_name strings you
		 *	 provede. So if you set color_name to "color" the
		 *	 corresponding shader input variables would have
		 *	 to be "color0" for the first color channer,
		 *	 "color1" for the second etc.
		 */
		void set_attribute_names(const char *position_name,
					 const char *normal_name,
					 const char *tangent_name,
					 const char *color_name,
					 const char *texture_coordinates_name,
					 const char *bone_ids_name,
					 const char *bone_weights_name);
		/**
		 * @brief Sets the animation speed.
		 * @param speed The speed multiplier
		 */
		void set_animation_speed(double speed);
		/**
		 * @brief Calculates new bone matrices based on the animation time
		 * @param time The current animation time. If time is greater than
		 * 	the duration of the animation 
		 * @return Returns true on success, false otherwise
		 */
		bool animate(float time);
		/**
		 * @brief Draws all associated meshes with the index buffer 0.
		 */
		void draw();
		/**
		 * @brief Draws all associated meshes with the index buffer 0.
		 * @param model_matrix The model matrix to use
		 *	  (NULL to use the model_matrix member)
		 */
		void draw(glm::mat4 *model_matrix);
};

}

#endif
