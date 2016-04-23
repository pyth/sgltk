#ifndef __MESH_H__
#define __MESH_H__

#include "core.h"
#include "shader.h"
#include "texture.h"

namespace sgltk {
	/**
	 * @struct Vertex
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

	std::string model_matrix_name;
	std::string view_matrix_name;
	std::string model_view_matrix_name;
	std::string view_proj_matrix_name;
	std::string model_view_projection_matrix_name;
	std::string normal_matrix_name;

	glm::mat4 *view_matrix;
	glm::mat4 *projection_matrix;

	GLuint vao;
	std::vector<GLuint> vbo;
	std::vector<GLuint> ibo;
	std::vector<int> num_indices;

	std::vector<int> bone_ids;
	std::vector<float> bone_weights;
public:
	/**
	 * @brief The mesh vertices
	 */
	std::vector<Vertex> vertices;
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
	 * @brief Sets the name of the model matrix in the shader
	 * @param name The name of the model matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "matrix.model_matrix"
	 */
	void set_model_matrix_name(std::string name);
	/**
	 * @brief Sets the name of the view matrix in the shader
	 * @param name The name of the view matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "matrix.model_matrix"
	 */
	void set_view_matrix_name(std::string name);
	/**
	 * @brief Sets the name of the model-view matrix in the shader
	 * @param name The name of the model-view matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "matrix.model_view"
	 */
	void set_model_view_matrix_name(std::string name);
	/**
	 * @brief Sets the name of the view-projection matrix in the shader
	 * @param name The name of the view-projection matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "matrix.view_proj"
	 */
	void set_view_proj_matrix_name(std::string name);
	/**
	 * @brief Sets the name of the model-view-projection matrix
	 * 	in the shader
	 * @param name The name of the model-view-projection matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "matrix.model_view_proj"
	 */
	void set_model_view_proj_name(std::string name);
	/**
	 * @brief Sets the name of the normal matrix in the shader
	 * @param name The name of the normal matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "matrix.normal_matrix"
	 */
	void set_normal_matrix_name(std::string normal_matrix_name);
	/**
	 * @brief Sets the name of the ambient color in the shader
	 * @param name The name of the ambient color component.
	 * 	The name is reset if string is empty.
	 * @note Default value is "color_ambient"
	 */
	void set_ambient_color_name(std::string name);
	/**
	 * @brief Sets the name of the diffuse color in the shader
	 * @param name The name of the diffuse color component.
	 * 	The name is reset if string is empty.
	 * @note Default value is "color_diffuse"
	 */
	void set_diffuse_color_name(std::string name);
	/**
	 * @brief Sets the name of the specular color in the shader
	 * @param name The name of the specular color component.
	 * 	The name is reset if string is empty.
	 * @note Default value is "color_specular"
	 */
	void set_specular_color_name(std::string name);
	/**
	 * @brief Sets the name of the shininess of the material
	 * 	in the shader
	 * @param name The name of the shininess component.
	 * 	The name is reset if string is empty.
	 * @note Default value is "shininess"
	 */
	void set_shininess_name(std::string name);
	/**
	 * @brief Sets the name of the shininess strength of the material
	 * 	in the shader
	 * @param name The name of the shininess strength component.
	 * 	The name is reset if string is empty.
	 * @note Default value is "shininess_strength"
	 */
	void set_shininess_strength_name(std::string name);
	/**
	 * @brief Sets the name of the ambient texture in the shader
	 * @param name The name of the ambient texture.
	 * 	The name is reset if string is empty.
	 */
	void set_ambient_texture_name(std::string name);
	/**
	 * @brief Sets the name of the diffuse texture in the shader
	 * @param name The name of the diffuse texture.
	 * 	The name is reset if string is empty.
	 */
	void set_diffuse_texture_name(std::string name);
	/**
	 * @brief Sets the name of the specular texture in the shader
	 * @param name The name of the specular texture.
	 * 	The name is reset if string is empty.
	 */
	void set_specular_texture_name(std::string name);
	/**
	 * @brief Sets the name of the shininess texture in the shader
	 * @param name The name of the shininess texture.
	 * 	The name is reset if string is empty.
	 */
	void set_shininess_texture_name(std::string name);
	/**
	 * @brief Sets the name of the emmisive texture in the shader
	 * @param name The name of the emmisive texture.
	 * 	The name is reset if string is empty.
	 */
	void set_emmisive_texture_name(std::string name);
	/**
	 * @brief Sets the name of the normals texture in the shader
	 * @param name The name of the normals texture.
	 * 	The name is reset if string is empty.
	 */
	void set_normals_texture_name(std::string name);
	/**
	 * @brief Sets the name of the displacement texture in the shader
	 * @param name The name of the displacement texture.
	 * 	The name is reset if string is empty.
	 */
	void set_displacement_texture_name(std::string name);
	/**
	 * @brief Sets the name of the opacity texture in the shader
	 * @param name The name of the opacity texture.
	 * 	The name is reset if string is empty.
	 */
	void set_opacity_texture_name(std::string name);
	/**
	 * @brief Sets the name of the lightmap texture in the shader
	 * @param name The name of the lightmap texture.
	 * 	The name is reset if string is empty.
	 */
	void set_lightmap_texture_name(std::string name);

	/**
	 * @brief Loads vertices into memory
	 * @param vertexdata The vertices to be loaded into memory
	 * @param size The size of the array in bytes
	 * @param usage A hint as to how the buffer will be accessed.
	 * 	Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 * @return Returns the index that the buffer was attached to
	 */
	template <typename T = Vertex>
	int attach_vertex_buffer(const void *vertexdata, unsigned int size,
				 GLenum usage = GL_STATIC_DRAW);
	/**
	 * @brief Loads vertices into memory
	 * @param vertexdata The vertices to be loaded into memory
	 * @param usage A hint as to how the buffer will be accessed.
	 * 	Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 * @return Returns the index that the buffer was attached to
	 */
	template <typename T = Vertex>
	int attach_vertex_buffer(const std::vector<T> *vertexdata,
				 GLenum usage = GL_STATIC_DRAW);
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
	 * @brief Computes the bounding box of the mesh
	 * @param vertexdata The vertices of the mesh
	 * @param pointer The pointer to the position vector in the vertex
	 * 	structure
	 */
	template <typename T = Vertex>
	void compute_bounding_box(const std::vector<T> *vertexdata, size_t pointer);

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

	bounding_box.push_back(glm::vec3(0, 0, 0));
	bounding_box.push_back(glm::vec3(0, 0, 0));

	model_matrix_name =			"matrix.model_matrix";
	view_matrix_name =			"matrix.view_matrix";
	model_view_matrix_name =		"matrix.model_view";
	view_proj_matrix_name =			"matrix.view_proj";
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
void Mesh<Vertex>::set_model_matrix_name(std::string name) {

	if(name.length() > 0)
		model_matrix_name = name;
	else
		model_matrix_name = "matrix.model_matrix";
}

template <typename Vertex>
void Mesh<Vertex>::set_view_matrix_name(std::string name) {

	if(name.length() > 0)
		view_matrix_name = name;
	else
		view_matrix_name = "matrix.view_matrix";
}

template <typename Vertex>
void Mesh<Vertex>::set_model_view_matrix_name(std::string name) {

	if(name.length() > 0)
		model_view_matrix_name = name;
	else
		model_view_matrix_name = "matrix.model_view";
}

template <typename Vertex>
void Mesh<Vertex>::set_view_proj_matrix_name(std::string name) {

	if(name.length() > 0)
		view_proj_matrix_name = name;
	else
		view_proj_matrix_name = "matrix.view_proj";
}

template <typename Vertex>
void Mesh<Vertex>::set_model_view_proj_name(std::string name) {

	if(name.length() > 0)
		model_view_projection_matrix_name = name;
	else
		model_view_projection_matrix_name = "matrix.model_view_proj";
}

template <typename Vertex>
void Mesh<Vertex>::set_normal_matrix_name(std::string name) {

	if(name.length() > 0)
		normal_matrix_name = name;
	else
		normal_matrix_name = "matrix.normal_matrix";
}

template <typename Vertex>
void Mesh<Vertex>::set_ambient_color_name(std::string name) {
	if(name.length() > 0)
		ambient_color_name = name;
	else
		ambient_color_name = "color_ambient";
}

template <typename Vertex>
void Mesh<Vertex>::set_diffuse_color_name(std::string name) {
	if(name.length() > 0)
		diffuse_color_name = name;
	else
		diffuse_color_name = "color_diffuse";
}

template <typename Vertex>
void Mesh<Vertex>::set_specular_color_name(std::string name) {
	if(name.length() > 0)
		specular_color_name = name;
	else
		specular_color_name = "color_specular";
}

template <typename Vertex>
void Mesh<Vertex>::set_shininess_name(std::string name) {
	if(name.length() > 0)
		shininess_name = name;
	else
		shininess_name = "shininess_name";
}

template <typename Vertex>
void Mesh<Vertex>::set_shininess_strength_name(std::string name) {
	if(name.length() > 0)
		shininess_strength_name = name;
	else
		shininess_strength_name = "shininess_strength";
}

template <typename Vertex>
void Mesh<Vertex>::set_ambient_texture_name(std::string name) {
	if(name.length() > 0)
		ambient_texture_name = name;
	else
		ambient_texture_name = "texture_ambient";
}

template <typename Vertex>
void Mesh<Vertex>::set_diffuse_texture_name(std::string name) {
	if(name.length() > 0)
		diffuse_texture_name = name;
	else
		diffuse_texture_name = "texture_diffuse";
}

template <typename Vertex>
void Mesh<Vertex>::set_specular_texture_name(std::string name) {
	if(name.length() > 0)
		specular_texture_name = name;
	else
		specular_texture_name = "texture_specular";
}

template <typename Vertex>
void Mesh<Vertex>::set_shininess_texture_name(std::string name) {
	if(name.length() > 0)
		shininess_texture_name = name;
	else
		shininess_texture_name = "texture_shininess";
}

template <typename Vertex>
void Mesh<Vertex>::set_emmisive_texture_name(std::string name) {
	if(name.length() > 0)
		emmisive_texture_name = name;
	else
		emmisive_texture_name = "texture_emmisive";
}

template <typename Vertex>
void Mesh<Vertex>::set_normals_texture_name(std::string name) {
	if(name.length() > 0)
		normals_texture_name = name;
	else
		normals_texture_name = "texture_normals";
}

template <typename Vertex>
void Mesh<Vertex>::set_displacement_texture_name(std::string name) {
	if(name.length() > 0)
		displacement_texture_name = name;
	else
		displacement_texture_name = "texture_displacement";
}

template <typename Vertex>
void Mesh<Vertex>::set_opacity_texture_name(std::string name) {
	if(name.length() > 0)
		opacity_texture_name = name;
	else
		opacity_texture_name = "texture_opacity";
}

template <typename Vertex>
void Mesh<Vertex>::set_lightmap_texture_name(std::string name) {
	if(name.length() > 0)
		lightmap_texture_name = name;
	else
		lightmap_texture_name = "texture_lightmap";
}

template <typename Vertex>
template <typename T>
int Mesh<Vertex>::attach_vertex_buffer(const void *vertexdata,
				       unsigned int size,
				       GLenum usage) {
	T *ptr = (T *)vertexdata;
	std::vector<T> tmp(ptr, ptr + size);
	return attach_vertex_buffer<T>(&tmp, usage);
}

template <typename Vertex>
template <typename T>
int Mesh<Vertex>::attach_vertex_buffer(const std::vector<T> *vertexdata,
				       GLenum usage) {
	GLuint buf;
	glGenBuffers(1, &buf);
	vbo.push_back(buf);

	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER, vertexdata->size() * sizeof(T),
		     vertexdata->data(), usage);

	return vbo.size() - 1;
}

template <typename Vertex>
template <typename T>
void Mesh<Vertex>::compute_bounding_box(const std::vector<T> *vertexdata, size_t pointer) {
	glm::vec3 *pos = (glm::vec3 *)(&(*vertexdata)[0] + pointer);
	bounding_box[0] = *pos;
	bounding_box[1] = *pos;
	for(size_t i = 1; i < vertexdata->size(); i++) {
		pos = (glm::vec3 *)(&(*vertexdata)[i] + pointer);
		if(pos->x < bounding_box[0].x)
			bounding_box[0].x = pos->x;
		if(pos->y < bounding_box[0].y)
			bounding_box[0].y = pos->y;
		if(pos->z < bounding_box[0].z)
			bounding_box[0].z = pos->z;
		if(pos->x > bounding_box[1].x)
			bounding_box[1].x = pos->x;
		if(pos->y > bounding_box[1].y)
			bounding_box[1].y = pos->y;
		if(pos->z > bounding_box[1].z)
			bounding_box[1].z = pos->z;
	}
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

	int loc = glGetAttribLocation(shader->program, attrib_name.c_str());
	if(loc == -1) {
		glBindVertexArray(0);
		return -2;
	}
	glEnableVertexAttribArray(loc);
	switch(type) {
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
		case GL_INT:
		case GL_UNSIGNED_INT:
			glVertexAttribIPointer(loc, size, type, stride,
						(void *)pointer);
			break;
		case GL_DOUBLE:
			glVertexAttribLPointer(loc, size, type, stride,
						(void *)pointer);
			break;
		default:
			glVertexAttribPointer(loc, size, type, GL_FALSE, stride,
						(void*)pointer);
			break;
	}

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

	int loc;
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
	glm::mat4 VP = (*projection_matrix) * (*view_matrix);

	shader->bind();
	loc = glGetUniformLocation(shader->program,
				       model_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(M));

	loc = glGetUniformLocation(shader->program,
				       view_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(*view_matrix));

	loc = glGetUniformLocation(shader->program,
				       model_view_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(MV));

	loc = glGetUniformLocation(shader->program,
				       view_proj_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(VP));

	loc = glGetUniformLocation(shader->program,
				   normal_matrix_name.c_str());
	glUniformMatrix3fv(loc, 1, false, glm::value_ptr(NM));

	loc = glGetUniformLocation(shader->program,
				   model_view_projection_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(MVP));

	loc = glGetUniformLocation(shader->program, ambient_color_name.c_str());
	glUniform4f(loc, color_ambient.x, color_ambient.y, color_ambient.z,
		    color_ambient.w);

	loc = glGetUniformLocation(shader->program, diffuse_color_name.c_str());
	glUniform4f(loc, color_diffuse.x, color_diffuse.y, color_diffuse.z,
		    color_diffuse.w);

	loc = glGetUniformLocation(shader->program, specular_color_name.c_str());
	glUniform4f(loc, color_specular.x, color_specular.y, color_specular.z,
		    color_specular.w);

	loc = glGetUniformLocation(shader->program, shininess_name.c_str());
	glUniform1f(loc, shininess);

	loc = glGetUniformLocation(shader->program, shininess_strength_name.c_str());
	glUniform1f(loc, shininess_strength);

	unsigned int num_textures = 0;
	for(unsigned int i = 0; i < textures_ambient.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				ambient_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = ambient_texture_name + '[' +
				std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_ambient[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_diffuse.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				diffuse_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = diffuse_texture_name + '[' +
				std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
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
}

#endif
