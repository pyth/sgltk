#ifndef __MESH_H__
#define __MESH_H__

#include "core.h"
#include "shader.h"
#include "texture.h"

namespace sgltk {
#pragma pack(push, 0)
	/**
	 * @brief A basic vertex structure 
	 */
	typedef struct Vertex {
		/**
		 * @brief Vertex position
		 */
		glm::vec4 position;
		/**
		 * @brief Vertex normal
		 */
		glm::vec3 normal;
		/**
		 * @brief Vertex tangent
		 */
		glm::vec4 tangent;
		/**
		 * @brief Vertex color
		 */
		glm::vec4 color;
		/**
		 * @brief Vertex texture coordinates
		 */
		glm::vec3 tex_coord;

		Vertex() {
			position = {0, 0, 0, 1};

			normal = {0, 0, 0};

			tangent = {0, 0, 0, 1};

			color = {0, 0, 0, 0};
		};

		/**
		 * @param p	Vertex position
		 * @param n	Vertex normal
		 */
		Vertex(glm::vec3 p, glm::vec3 n) {
			position = glm::vec4(p, 1);

			normal = n;

			tangent = {0, 0, 0, 1};

			color = {0, 0, 0, 0};

			tex_coord = {0, 0, 0};
		};

		/**
		 * @param p	Vertex position
		 * @param n	Vertex normal
		 * @param tc	Texture coordinates
		 */
		Vertex(glm::vec3 p, glm::vec3 n, glm::vec3 tc) {
			position = glm::vec4(p, 1);

			normal = n;

			tangent = {0, 0, 0, 1};

			color = {0, 0, 0, 0};

			tex_coord = tc;
		};

		/**
		 * @param p	Vertex position
		 * @param n	Vertex normal
		 * @param t	Vertex tangent
		 * @param tc	Texture coordinates
		 */
		Vertex(glm::vec3 p, glm::vec3 n, glm::vec3 t, glm::vec3 tc) {
			position = glm::vec4(p, 1);

			normal = n;

			tangent = glm::vec4(t, 1);

			color = {0, 0, 0, 0};

			tex_coord = tc;
		};

		/**
		 * @param p	Vertex position
		 * @param n	Vertex normal
		 * @param t	Vertex tangent
		 * @param c	Vertex color
		 * @param tc	Texture coordinates
		 */
		Vertex(glm::vec3 p, glm::vec3 n, glm::vec3 t,
		       glm::vec4 c, glm::vec3 tc) {
			position = glm::vec4(p, 1);

			normal = n;

			tangent = glm::vec4(t, 1);

			color = c;

			tex_coord = tc;
		};
	} Vertex;

#pragma pack(pop)
}

/**
 * @class Mesh
 * @brief Manages meshes
 */
template <typename Vertex = sgltk::Vertex>
class Mesh {
	Shader *shader;

	std::string ambient_color_name;
	std::string diffuse_color_name;
	std::string specular_color_name;
	std::string shininess_name;
	std::string shininess_strength_name;

	std::string ambient_texture_name;
	std::string diffuse_texture_name;
	std::string specular_texture_name;
	std::string shininess_texture_name;
	std::string emmisive_texture_name;
	std::string normals_texture_name;
	std::string displacement_texture_name;
	std::string opacity_texture_name;
	std::string lightmap_texture_name;

	std::string model_view_matrix_name;
	std::string model_view_projection_matrix_name;
	std::string normal_matrix_name;

	glm::mat4 *view_matrix;
	glm::mat4 *projection_matrix;

	GLuint vao;
	std::vector<GLuint> vbo;
	std::vector<GLuint> ibo;
	std::vector<int> num_indices;

	void compute_bounding_box(const std::vector<Vertex> *vertexdata);
public:
	/**
	 * @brief The bounding box
	 */
	std::vector<glm::vec3> bounding_box;
	/**
	 * @brief The model matrix
	 */
	glm::mat4 model_matrix;
	/**
	 * @brief The shininess of the material
	 */
	float shininess;
	/**
	 * @brief The strength of the shininess of the material
	 */
	float shininess_strength;
	/**
	 * @brief The ambient color component of the material
	 */
	glm::vec4 color_ambient;
	/**
	 * @brief The diffuse color component of the material
	 */
	glm::vec4 color_diffuse;
	/**
	 * @brief The specular color component of the material
	 */
	glm::vec4 color_specular;
	/**
	 * @brief The ambient textures
	 */
	std::vector<Texture *> textures_ambient;
	/**
	 * @brief The diffuse textures
	 */
	std::vector<Texture *> textures_diffuse;
	/**
	 * @brief The specular textures
	 */
	std::vector<Texture *> textures_specular;
	/**
	 * @brief The shininess textures
	 */
	std::vector<Texture *> textures_shininess;
	/**
	 * @brief The emmisive textures
	 */
	std::vector<Texture *> textures_emmisive;
	/**
	 * @brief The normals textures
	 */
	std::vector<Texture *> textures_normals;
	/**
	 * @brief The displacement textures
	 */
	std::vector<Texture *> textures_displacement;
	/**
	 * @brief The opacity textures
	 */
	std::vector<Texture *> textures_opacity;
	/**
	 * @brief The lightmap textures
	 */
	std::vector<Texture *> textures_lightmap;

	/**
	 * @brief Indicates that the mesh should be drawn as a wireframe
	 */
	bool wireframe;
	/**
	 * @brief Indicates that the mesh should not be drawn using
	 * 	  back face culling
	 */
	bool twosided;
	//std::vector<Texture> textures;
	//std::vector<float> texture_blend;

	Mesh();
	~Mesh();

	/**
	 * @brief Specifies the shader to use to render the mesh
	 * @param shader The shader to be used to render the mesh
	 */
	void setup_shader(Shader *shader);
	/**
	 * @brief Sets up the view and projection matrices that will be used
	 * 	  by the mesh
	 * @param view_matrix The view matrix
	 * @param projection_matrix The projection matrix
	 */
	void setup_camera(glm::mat4 *view_matrix,
			  glm::mat4 *projection_matrix);
	/**
	 * @brief Specifies the names of the matrices in the shader
	 * @param model_view_matrix_name The name of the model-view
	 *	matrixin in the shader
	 * @param model_view_projection_matrix_name The name of the
	 *	model-view-projection matrixin in the shader
	 * @param normal_matrix_name The name of the normal matrix
	 *	in the shader
	 */
	void setup_matrices(std::string model_view_matrix_name,
			    std::string model_view_projection_matrix_name,
			    std::string normal_matrix_name);
	/**
	 * @brief Specifies the names of the material components in the shader
	 * @param ambient_color_name The name of the ambient color component
	 * @param diffuse_color_name The name of the diffuse color component
	 * @param specular_color_name The name of the specular color component
	 * @param shininess_name The name of the specular exponent
	 * @param shininess_strength_name The name of the specular intensity
	 */
	void setup_material(std::string ambient_color_name,
			    std::string diffuse_color_name,
			    std::string specular_color_name,
			    std::string shininess_name,
			    std::string shininess_strength_name);
	/**
	 * @brief Specifies the names of the material components in the shader
	 * @param ambient_texture_name The name of the ambient texture
	 * @param diffuse_texture_name The name of the diffuse texture
	 * @param specular_texture_name The name of the specular texture
	 * @param shininess_texture_name The name of the shininess texture
	 * @param emmisive_texture_name The name of the emmisive texture
	 * @param normals_texture_name The name of the normals texture
	 * @param displacement_texture_name The name of the displacement texture
	 * @param opacity_texture_name The name of the opacity texture
	 * @param lightmap_texture_name The name of the lightmap texture
	 */
	void setup_textures(std::string ambient_texture_name,
			    std::string diffuse_texture_name,
			    std::string specular_texture_name,
			    std::string shininess_texture_name,
			    std::string emmisive_texture_name,
			    std::string normals_texture_name,
			    std::string displacement_texture_name,
			    std::string opacity_texture_name,
			    std::string lightmap_texture_name);
	/**
	 * @brief Loads vertices into memory
	 * @param size The size of the array in bytes
	 * @param vertexdata The vertices to be loaded into memory
	 * @return Returns the index that the buffer was attached to
	 */
	int attach_vertex_buffer(unsigned int size, const void *vertexdata);
	/**
	 * @brief Loads vertices into memory
	 * @param vertexdata The vertices to be loaded into memory
	 * @return Returns the index that the buffer was attached to
	 */
	template <typename T = Vertex>
	int attach_vertex_buffer(const std::vector<T> *vertexdata);
	/**
	 * @brief Sets pointers to vertex attribures
	 * @param attrib_name	The name as defined in the shader
	 * @param buffer_index	The index of the buffer that contains
	 *			the attribute
	 * @param size		Number of elements
	 * @param type		Element type
	 * @param stride	Memory offset between vertices
	 * @param pointer	The offset of the attribute in the
	 * 			vertex structure
	 * @return	Returns 0 on success, -1 if no shader was
	 * 		specified for the mesh, -2 if the vertex attribute
	 * 		could not be found
	 */
	int set_vertex_attribute(std::string attrib_name,
				 unsigned int buffer_index,
				 GLint size,
				 GLenum type,
				 GLsizei stride,
				 const GLvoid *pointer);
	/**
	 * @brief Attaches an index array to the mesh
	 * @param indices Indices describing the topology of the mesh
	 * You can attach multiple index arrays
	 */
	void attach_index_buffer(const std::vector<unsigned short> *indices);

	/**
	 * @brief Renders the mesh using the first index buffer
	 * @param mode Specifies the primitive that will be created from
	 * 	vertices
	 */
	void draw(GLenum mode);

	/**
	 * @brief Renders the mesh using the first index buffer and the
	 * 	  model_matrix member as model matrix
	 * @param mode Specifies the primitive that will be created from
	 * 	  vertices
	 * @param index_buffer The index buffer to use
	 */
	void draw(GLenum mode, unsigned int index_buffer);

	/**
	 * @brief Renders the mesh using the first index buffer
	 * @param mode Specifies the primitive that will be created from
	 * 	  vertices
	 * @param model_matrix The model matrix to use
	 *	  (NULL to use the model_matrix member)
	 */
	void draw(GLenum mode, glm::mat4 *model_matrix);

	/**
	 * @brief Renders the mesh
	 * @param mode Specifies the primitive that will be created from
	 * 	  vertices
	 * @param index_buffer The index buffer to use
	 * @param model_matrix The model matrix to use
	 *	  (NULL to use the model_matrix member)
	 */
	void draw(GLenum mode, unsigned int index_buffer,
		  glm::mat4 *model_matrix);
};

template <typename Vertex>
Mesh<Vertex>::Mesh() {
	sgltk::init_lib();
	model_matrix = glm::mat4(1.0);
	shader = NULL;
	glGenVertexArrays(1, &vao);

	model_view_matrix_name =		"matrix.model_view";
	model_view_projection_matrix_name =	"matrix.model_view_proj";
	normal_matrix_name =			"matrix.normal_matrix";

	ambient_color_name =			"color_ambient";
	diffuse_color_name =			"color_diffuse";
	specular_color_name =			"color_specular";
	shininess_name =			"shininess";
	shininess_strength_name =		"shininess_strength";

	ambient_texture_name =			"texture_ambient";
	diffuse_texture_name =			"texture_diffuse";
	specular_texture_name =			"texture_specular";
	shininess_texture_name =		"texture_shininess";
	emmisive_texture_name =			"texture_emmisive";
	normals_texture_name =			"texture_normals";
	displacement_texture_name =		"texture_displacement";
	opacity_texture_name =			"texture_opacity";
	lightmap_texture_name =			"texture_lightmap";

	shininess = 0.0;
	shininess_strength = 1.0;
	color_ambient = glm::vec4(0, 0, 0, 1);
	color_diffuse = glm::vec4(0, 0, 0, 1);
	color_specular = glm::vec4(0, 0, 0, 1);
}

template <typename Vertex>
Mesh<Vertex>::~Mesh() {
	glDeleteBuffers(vbo.size(), vbo.data());
	glDeleteBuffers(ibo.size(), ibo.data());
	glDeleteVertexArrays(1, &vao);
}

template <typename Vertex>
void Mesh<Vertex>::setup_shader(Shader *shader) {
	this->shader = shader;
}

template <typename Vertex>
void Mesh<Vertex>::setup_camera(glm::mat4 *view_matrix,
				glm::mat4 *projection_matrix) {
	this->view_matrix = view_matrix;
	this->projection_matrix = projection_matrix;
}

template <typename Vertex>
void Mesh<Vertex>::setup_matrices(std::string model_view_matrix_name,
				  std::string model_view_projection_matrix_name,
				  std::string normal_matrix_name) {

	if(model_view_matrix_name.length() > 0)
		this->model_view_matrix_name = model_view_matrix_name;
	
	if(model_view_projection_matrix_name.length() > 0)
		this->model_view_projection_matrix_name =
			model_view_projection_matrix_name;

	if(normal_matrix_name.length() > 0)
		this->normal_matrix_name = normal_matrix_name;
}

template <typename Vertex>
void Mesh<Vertex>::setup_material(std::string ambient_color_name,
				  std::string diffuse_color_name,
				  std::string specular_color_name,
				  std::string shininess_name,
				  std::string shininess_strength_name) {

	if(ambient_color_name.length() > 0)
		this->ambient_color_name = ambient_color_name;

	if(diffuse_color_name.length() > 0)
		this->diffuse_color_name = diffuse_color_name;

	if(specular_color_name.length() > 0)
		this->specular_color_name = specular_color_name;

	if(shininess_name.length() > 0)
		this->shininess_name = shininess_name;

	if(shininess_strength_name.length() > 0)
		this->shininess_strength_name = shininess_strength_name;
}

template <typename Vertex>
void Mesh<Vertex>::setup_textures(std::string ambient_texture_name,
				  std::string diffuse_texture_name,
				  std::string specular_texture_name,
				  std::string shininess_texture_name,
				  std::string emmisive_texture_name,
				  std::string normals_texture_name,
				  std::string displacement_texture_name,
				  std::string opacity_texture_name,
				  std::string lightmap_texture_name) {

	if(ambient_texture_name.length() > 0)
		this->ambient_texture_name = ambient_texture_name;

	if(diffuse_texture_name.length() > 0)
		this->diffuse_texture_name = diffuse_texture_name;

	if(specular_texture_name.length() > 0)
		this->specular_texture_name = specular_texture_name;

	if(shininess_texture_name.length() > 0)
		this->shininess_texture_name = shininess_texture_name;

	if(emmisive_texture_name.length() > 0)
		this->emmisive_texture_name = emmisive_texture_name;

	if(normals_texture_name.length() > 0)
		this->normals_texture_name = normals_texture_name;

	if(displacement_texture_name.length() > 0)
		this->displacement_texture_name = displacement_texture_name;

	if(opacity_texture_name.length() > 0)
		this->opacity_texture_name = opacity_texture_name;

	if(lightmap_texture_name.length() > 0)
		this->lightmap_texture_name = lightmap_texture_name;

}

template <typename Vertex>
int Mesh<Vertex>::attach_vertex_buffer(unsigned int size, const void *vertexdata) {
	//glBindVertexArray(vao);
	GLuint buf;
	glGenBuffers(1, &buf);
	vbo.push_back(buf);

	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER, size, vertexdata, GL_STATIC_DRAW);

	return vbo.size() - 1;
	//glBindVertexArray(0);
}

template <typename Vertex>
template <typename T>
int Mesh<Vertex>::attach_vertex_buffer(const std::vector<T> *vertexdata) {
	//glBindVertexArray(vao);
	GLuint buf;
	glGenBuffers(1, &buf);
	vbo.push_back(buf);

	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER, vertexdata->size() * sizeof(T),
		     vertexdata->data(), GL_STATIC_DRAW);

	return vbo.size() - 1;
	//glBindVertexArray(0);
}

template <typename Vertex>
void Mesh<Vertex>::compute_bounding_box(const std::vector<Vertex> *vertexdata) {
}

template <typename Vertex>
int Mesh<Vertex>::set_vertex_attribute(std::string attrib_name,
				       unsigned int buffer_index,
				       GLint size,
				       GLenum type,
				       GLsizei stride,
				       const GLvoid *pointer) {
	if(!shader) {
		return -1;
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[buffer_index]);

	int loc = glGetAttribLocation(shader->shader, attrib_name.c_str());
	if(loc == -1) {
		glBindVertexArray(0);
		return -2;
	}
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, size, type, GL_FALSE, stride,
			      (void*)pointer);

	glBindVertexArray(0);
	return 0;
}

template <typename Vertex>
void Mesh<Vertex>::attach_index_buffer(const std::vector<unsigned short> *indices) {
	num_indices.push_back(indices->size());
	
	GLuint index;
	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		     indices->size() * sizeof(unsigned short),
		     indices->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	ibo.push_back(index);
}

template <typename Vertex>
void Mesh<Vertex>::draw(GLenum mode) {
	draw(mode, 0, NULL);
}

template <typename Vertex>
void Mesh<Vertex>::draw(GLenum mode, unsigned int index_buffer) {
	draw(mode, index_buffer, NULL);
}

template <typename Vertex>
void Mesh<Vertex>::draw(GLenum mode, glm::mat4 *model_matrix) {
	draw(mode, 0, model_matrix);
}

template <typename Vertex>
void Mesh<Vertex>::draw(GLenum mode, unsigned int index_buffer,
			glm::mat4 *model_matrix) {

	if(!shader) {
		std::cerr << "Error: No shader specified" << std::endl;
		return;
	}

	glm::mat4 M;
	glm::mat4 MV;
	glm::mat3 NM;
	if(model_matrix)
		M = *model_matrix;
	else
		M = this->model_matrix;

	MV = (*view_matrix) * M;
	NM = glm::mat3(glm::transpose(glm::inverse(MV)));
	glm::mat4 MVP = (*projection_matrix) * MV;

	shader->bind();
	int loc = glGetUniformLocation(shader->shader,
				       model_view_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(MV));

	loc = glGetUniformLocation(shader->shader,
				   normal_matrix_name.c_str());
	glUniformMatrix3fv(loc, 1, false, glm::value_ptr(NM));

	loc = glGetUniformLocation(shader->shader,
				   model_view_projection_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(MVP));

	loc = glGetUniformLocation(shader->shader, ambient_color_name.c_str());
	glUniform4f(loc, color_ambient.x, color_ambient.y, color_ambient.z,
		    color_ambient.w);

	loc = glGetUniformLocation(shader->shader, diffuse_color_name.c_str());
	glUniform4f(loc, color_diffuse.x, color_diffuse.y, color_diffuse.z,
		    color_diffuse.w);

	loc = glGetUniformLocation(shader->shader, specular_color_name.c_str());
	glUniform4f(loc, color_specular.x, color_specular.y, color_specular.z,
		    color_specular.w);

	loc = glGetUniformLocation(shader->shader, shininess_name.c_str());
	glUniform1f(loc, shininess);

	loc = glGetUniformLocation(shader->shader, shininess_strength_name.c_str());
	glUniform1f(loc, shininess_strength);

	unsigned int num_textures = 0;
	for(unsigned int i = 0; i < textures_ambient.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->shader,
				ambient_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = ambient_texture_name + '[' +
				std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->shader,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_ambient[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_diffuse.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->shader,
				diffuse_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = diffuse_texture_name + '[' +
				std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->shader,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_diffuse[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_specular.size(); i++) {
		textures_specular[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_shininess.size(); i++) {
		textures_shininess[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_emmisive.size(); i++) {
		textures_emmisive[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_normals.size(); i++) {
		textures_normals[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_displacement.size(); i++) {
		textures_displacement[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_opacity.size(); i++) {
		textures_opacity[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_lightmap.size(); i++) {
		textures_lightmap[i]->bind(num_textures);
		num_textures++;
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[index_buffer]);
	glDrawElements(mode, num_indices[index_buffer],
		       GL_UNSIGNED_SHORT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shader->unbind();
}
#endif
