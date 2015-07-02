#include "shader.h"

using namespace std;

Shader::Shader() {
	modify = true;
	shader = glCreateProgram();
}

Shader::~Shader() {
	glDeleteProgram(shader);
}

bool Shader::attach(const char *filename, GLenum type) {
	GLint compiled;
	char infoLog[4096];
	int infoLogLength;
	SDL_RWops *file;
	const char *code;
	char *buf;
	GLuint tmp;
	int size;

	file = SDL_RWFromFile(filename, "r+b");
	if(!file) {
		cerr << "Could not open shader file: " << filename << " - "
		     << SDL_GetError() << endl;
		return false;
	}
	size = (int)SDL_RWsize(file);
	buf = new char[size];
	SDL_RWread(file, buf, size, 1);
	SDL_RWclose(file);
	code = buf;

	tmp = glCreateShader(type);
	glShaderSource(tmp, 1, &code, &size);
	glCompileShader(tmp);
	glGetShaderiv(tmp, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
		glGetShaderInfoLog(tmp, sizeof(infoLog), &infoLogLength,
				   infoLog);
		if(infoLogLength > 0) {
			printf("CompileShader() infoLog %s \n%s\n", filename,
			       infoLog);
			return false;
		}
	}

	glAttachShader(shader, tmp);
	if(modify) {
		shader_paths.push_back(filename);
		shader_map[filename] = type;
	}

	glDeleteShader(tmp);
	delete buf;
	return true;
}

void Shader::bind_uniform(char *name, UniformType type, void *data) {
	/*int location = glGetUniformLocation(shader, name);
	switch(type) {
	case INTEGER:
		glUniform1i(location, *((int*)data));
		break;
	case FLOAT:
		glUniform1f(location, *((float*)data));
		break;
	case VECTOR3F:
		glUniform3fv(location, 1, glm::value_ptr(*(glm::vec3*)data));
		break;
	case VECTOR4F:
		glUniform4fv(location, 1, glm::value_ptr(*(glm::vec4*)data));
		break;
	case MATRIX3:
		glUniformMatrix3fv(location, 1, false, glm::value_ptr(*(glm::mat3*)data));
		break;
	case MATRIX4:
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(*(glm::mat4*)data));
		break;
	default:
		break;
	}*/
}

void Shader::recompile() {
	modify = false;
	unbind();
	glDeleteProgram(shader);
	shader = glCreateProgram();
	for(int i = 0; i < shader_paths.size(); i++) {
		attach(shader_paths[i], shader_map[shader_paths[i]]);
	}
	link();
	modify = true;
}

void Shader::link() {
	glLinkProgram(shader);
}

void Shader::bind() {
	glUseProgram(shader);
}

void Shader::unbind() {
	glUseProgram(0);
}
