#include "shader.h"

using namespace sgltk;

std::vector<std::string> Shader::paths = {"./"};

Shader::Shader() {
	modify = true;
	program = glCreateProgram();
	if(!program) {
		std::string error("Error creating new shader program");
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}
}

Shader::~Shader() {
	glDeleteProgram(program);
}

bool Shader::attach_file(const std::string& filename, GLenum type) {
	std::string path;
	GLint compiled;
	char infoLog[4096];
	int infoLogLength;
	SDL_RWops *file;
	const char *code;
	GLuint tmp;
	GLint size;

	if((filename.length() > 1 && filename[0] == '/') ||
			(filename.length() > 2 && filename[1] == ':')) {
		//absolute path
		file = SDL_RWFromFile(filename.c_str(), "r+b");
		path = filename;
	} else {
		//relative path
		for(unsigned int i = 0; i < paths.size(); i++) {
			file = SDL_RWFromFile((paths[i] + filename).c_str(),
						"r+b");
			if(file) {
				path = paths[i] + filename;
				break;
			}
		}
	}

	if(!file) {
		App::error_string.push_back(std::string("Could not open shader file: ")
				+ filename + std::string(" - ") + SDL_GetError());
		return false;
	}
	size = (int)SDL_RWsize(file);
	std::vector<char> buf(size);
	SDL_RWread(file, buf.data(), size, 1);
	SDL_RWclose(file);
	code = buf.data();

	tmp = glCreateShader(type);
	glShaderSource(tmp, 1, &code, &size);
	glCompileShader(tmp);
	glGetShaderiv(tmp, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
		glGetShaderInfoLog(tmp, sizeof(infoLog), &infoLogLength,
				   infoLog);
		if(infoLogLength > 0) {
			printf("CompileShader() infoLog %s \n%s\n",
				filename.c_str(), infoLog);
			return false;
		}
	}

	glAttachShader(program, tmp);
	if(modify) {
		shader_path_map[path] = type;
	}

	glDeleteShader(tmp);
	return true;
}

bool Shader::attach_string(const std::string& shader_string, GLenum type) {
	GLint compiled;
	const char *string_ptr = shader_string.c_str();
	int size = shader_string.length();
	char infoLog[4096];
	int infoLogLength;

	GLuint tmp = glCreateShader(type);
	glShaderSource(tmp, 1, &(string_ptr), &(size));
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

	glAttachShader(program, tmp);
	if(modify) {
		shader_string_map[shader_string] = type;
	}

	glDeleteShader(tmp);
	return true;
}

void Shader::recompile() {
	modify = false;
	unbind();
	glDeleteProgram(program);
	program = glCreateProgram();
	for(const std::pair<std::string, GLenum>& it : shader_path_map) {
		attach_file(it.first, it.second);
	}
	for(const std::pair<std::string, GLenum>& it : shader_string_map) {
		attach_string(it.first, it.second);
	}
	link();
	modify = true;
}

void Shader::link() {
	glLinkProgram(program);
}

void Shader::bind() {
	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&saved_program);
	glUseProgram(program);
}

void Shader::unbind() {
	glUseProgram(saved_program);
}

void Shader::add_path(std::string path) {
	if(path[path.length() - 1] != '/')
		path += '/';

	if(std::find(paths.begin(), paths.end(), path) == paths.end())
		paths.push_back(path);
}
