#include "shader.h"

using namespace std;

Shader::Shader() {
	modify = true;
	shader = glCreateProgram();
}

Shader::~Shader() {
	glDeleteProgram(shader);
}

bool Shader::attach_file(const char *filename, GLenum type) {
	GLint compiled;
	char infoLog[4096];
	int infoLogLength;
	SDL_RWops *file;
	const char *code;
	char *buf;
	GLuint tmp;
	GLint size;

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
		shader_path_map[filename] = type;
	}

	glDeleteShader(tmp);
	delete buf;
	return true;
}

bool Shader::attach_string(const char *shader_string, GLint size, GLenum type) {
	GLint compiled;
	char infoLog[4096];
	int infoLogLength;

	GLuint tmp = glCreateShader(type);
	glShaderSource(tmp, 1, &shader_string, &size);
	glCompileShader(tmp);
	glGetShaderiv(tmp, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
		glGetShaderInfoLog(tmp, sizeof(infoLog), &infoLogLength,
				   infoLog);
		if(infoLogLength > 0) {
			printf("CompileShader() infoLog \n%s\n",
			       infoLog);
			return false;
		}
	}

	glAttachShader(shader, tmp);
	if(modify) {
		shader_string_map[shader_string] = type;
	}

	glDeleteShader(tmp);
	return true;
}

void Shader::bind_uniform(char *name, sgltk::UNIFORM_TYPE type, void *data) {
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
	for(std::map<const char *, GLenum>::iterator it = shader_path_map.begin(); it != shader_path_map.end(); it++) {
		attach_file(it->first, it->second);
	}
	for(std::map<const char *, GLenum>::iterator it = shader_string_map.begin(); it != shader_string_map.end(); it++) {
		attach_string(it->first, strlen(it->first)+1, it->second);
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
