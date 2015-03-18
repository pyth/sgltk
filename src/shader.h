#ifndef __SHADER_H__
#define __SHADER_H__

#include "core.h"

enum UniformType {
	INTEGER1,
	FLOAT1,
	VECTOR3F,
	VECTOR4F,
	MATRIX3,
	MATRIX4
};

class Shader {
public:
	GLuint shader;

	Shader();
	~Shader();

	bool attach(const char *filename, GLenum type);
	void bind_uniform(UniformType type, char *name, void *data);
	void link();
	void bind();
	void unbind();
};

#endif
