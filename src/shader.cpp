#include "shader.h"

using namespace sgltk;

std::vector<std::string> Shader::paths = {"./"};

Shader::Shader() {
	modify = true;
	program = glCreateProgram();
}

Shader::~Shader() {
	glDeleteProgram(program);
}

bool Shader::attach_file(std::string filename, GLenum type) {
	std::string path;
	GLint compiled;
	char infoLog[4096];
	int infoLogLength;
	SDL_RWops *file;
	const char *code;
	char *buf;
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
		std::cerr << "Could not open shader file: " << filename << " - "
		     << SDL_GetError() << std::endl;
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

	glAttachShader(program, tmp);
	if(modify) {
		shader_path_map[path] = type;
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
	for(std::map<std::string, GLenum>::iterator it = shader_path_map.begin(); it != shader_path_map.end(); it++) {
		attach_file(it->first, it->second);
	}
	for(std::map<const char *, GLenum>::iterator it = shader_string_map.begin(); it != shader_string_map.end(); it++) {
		attach_string(it->first, strlen(it->first)+1, it->second);
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
