#ifndef __SCENE_H__
#define __SCENE_H__

#include "core.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

/**
 * @class Scene
 * @brief Manages imported scenes
 */
class Scene {
	Assimp::Importer importer;
	const aiScene *scene;
	Shader *shader;
	std::vector<Mesh<> *> meshes;

	const char *model_view_matrix_name;
	const char *model_view_projection_matrix_name;
	const char *normal_matrix_name;
	glm::mat4 *view_matrix;
	glm::mat4 *projection_matrix;

	void traverse_nodes(aiNode *start_node, aiMatrix4x4 *parent_trafo);
	void create_mesh(aiMesh *mesh, aiMatrix4x4 *paren_trafo);
	void ai_to_glm_mat4(aiMatrix4x4 *in, glm::mat4 &out);
	public:
		glm::mat4 model_matrix;

		/**
		 * @param path The path to the file to load
		 * @param shader The shader user to render the loaded meshes
		 * @param model_view_matrix_name The model-view matrix name in the shader
		 * @param model_view_projection_matrix_name The model-view-projection
		 *	  matrix name in the shader
		 * @param normal_matrix_name The normal matrix name in the shader
		 * @param view_matrix The view matrix
		 * @param projection_matrix The projection matrix
		 */
		Scene(const char *path, Shader *shader,
		      glm::mat4 *view_matrix,
		      glm::mat4 *projection_matrix);
		~Scene();

		/**
		 * @brief Sets pointers to vertex attribures
		 * @param attrib_name	The name as defined in the shader
		 * @param size		Number of elements
		 * @param type		Element type
		 * @param stride	Memory offset between vertices
		 * @param pointer	The offset of the attribute in the
		 *				vertex structure
		 */
		void set_vertex_attribute(const char *attrib_name, GLint size,
					  GLenum type, GLsizei stride,
					  const GLvoid *pointer);
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

#endif
