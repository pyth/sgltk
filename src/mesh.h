#ifndef __MESH_H__
#define __MESH_H__

#include "core.h"
#include "shader.h"

struct Vertex {
	GLfloat position[4];
	GLfloat normal[4];
	GLfloat texcoord[2];

	Vertex(glm::vec3 p, glm::vec3 n) {
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;
		normal[3] = 1;

	};

	Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t) {
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;
		normal[3] = 1;

		texcoord[0] = t.x;
		texcoord[1] = t.y;
	};
};

class Mesh {
	Shader *shader;
	GLuint vao;
	GLuint vbo;
	std::vector<GLuint> ibo;
public:
	glm::mat4 trafo;

	std::vector<int> num_indices;
	int num_index_buffers;
	int num_vertex_attributes;

	Mesh();
	~Mesh();

	void use_shader(Shader *shader);
	void attach_vertex_array(const std::vector<Vertex> *vertexdata);
	void set_vertex_attribute(const char *attrib_name, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void attach_index_array(const std::vector<unsigned short> *indices);

	void draw(GLenum mode);
	void draw(GLenum mode, unsigned int index_buffer);
};

#endif
