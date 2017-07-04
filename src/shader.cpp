#include "shader.h"

using namespace sgltk;

int Shader::counter = 0;
int Shader::bound = -1;
std::vector<std::string> Shader::paths = {"./"};

void Shader::add_path(std::string path) {
	if(path[path.length() - 1] != '/')
		path += '/';

	if(std::find(paths.begin(), paths.end(), path) == paths.end())
		paths.push_back(path);
}

Shader::Shader() {
	if(counter < 0) {
		std::string error("Error creating new shader program");
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}
	id = counter;
	counter++;
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
			file = SDL_RWFromFile((paths[i] + filename).c_str(), "r+b");
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
			std::cerr << "CompileShader() infoLog " <<
				filename << std::endl << infoLog << std::endl;
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
			std::cerr << "CompileShader() infoLog " << std::endl
				<< infoLog << std::endl;
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

void Shader::set_transform_feedback_variables(std::vector<char *>& variables, GLenum buffer_mode) {
	glTransformFeedbackVaryings(program, variables.size(), variables.data(), buffer_mode);
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
	if(bound == id) {
		return;
	}
	bound = id;
	glUseProgram(program);
}

void Shader::unbind() {
	if(bound == id) {
		glUseProgram(0);
	}
	bound = -1;
}

int Shader::get_attribute_location(const std::string& name) {
	return glGetAttribLocation(program, name.c_str());
}

int Shader::get_uniform_location(const std::string& name) {
	return glGetUniformLocation(program, name.c_str());
}

void Shader::set_uniform_int(int location, int v0) {
	bind();
	glUniform1i(location, v0);
}

void Shader::set_uniform_uint(int location, unsigned int v0) {
	bind();
	glUniform1ui(location, v0);
}

void Shader::set_uniform_float(int location, float v0) {
	bind();
	glUniform1f(location, v0);
}

void Shader::set_uniform_int(const std::string& name, int v0) {
	int loc = get_uniform_location(name);
	set_uniform_int(loc, v0);
}

void Shader::set_uniform_uint(const std::string& name, unsigned int v0) {
	int loc = get_uniform_location(name);
	set_uniform_uint(loc, v0);
}

void Shader::set_uniform_float(const std::string& name, float v0) {
	int loc = get_uniform_location(name);
	set_uniform_float(loc, v0);
}

void Shader::set_uniform_int(int location, int v0,
					   int v1) {
	bind();
	glUniform2i(location, v0, v1);
}

void Shader::set_uniform_uint(int location, unsigned int v0,
					    unsigned int v1) {
	bind();
	glUniform2ui(location, v0, v1);
}

void Shader::set_uniform_float(int location, float v0,
					     float v1) {
	bind();
	glUniform2f(location, v0, v1);
}

void Shader::set_uniform_int(const std::string& name, int v0,
						      int v1) {
	int loc = get_uniform_location(name);
	set_uniform_int(loc, v0, v1);
}

void Shader::set_uniform_uint(const std::string& name, unsigned int v0,
						       unsigned int v1) {
	int loc = get_uniform_location(name);
	set_uniform_uint(loc, v0, v1);
}

void Shader::set_uniform_float(const std::string& name, float v0,
							float v1) {
	int loc = get_uniform_location(name);
	set_uniform_float(loc, v0, v1);
}

void Shader::set_uniform_int(int location, int v0,
					   int v1,
					   int v2) {
	bind();
	glUniform3i(location, v0, v1, v2);
}

void Shader::set_uniform_uint(int location, unsigned int v0,
					    unsigned int v1,
					    unsigned int v2) {
	bind();
	glUniform3ui(location, v0, v1, v2);
}

void Shader::set_uniform_float(int location, float v0,
					     float v1,
					     float v2) {
	bind();
	glUniform3f(location, v0, v1, v2);
}

void Shader::set_uniform_int(const std::string& name, int v0,
						      int v1,
						      int v2) {
	int loc = get_uniform_location(name);
	set_uniform_int(loc, v0, v1, v2);
}

void Shader::set_uniform_uint(const std::string& name, unsigned int v0,
						       unsigned int v1,
						       unsigned int v2) {
	int loc = get_uniform_location(name);
	set_uniform_uint(loc, v0, v1, v2);
}

void Shader::set_uniform_float(const std::string& name, float v0,
							float v1,
							float v2) {
	int loc = get_uniform_location(name);
	set_uniform_float(loc, v0, v1, v2);
}

void Shader::set_uniform_int(int location, int v0,
					   int v1,
					   int v2,
					   int v3) {
	bind();
	glUniform4i(location, v0, v1, v2, v3);
}

void Shader::set_uniform_uint(int location, unsigned int v0,
					    unsigned int v1,
					    unsigned int v2,
					    unsigned int v3) {
	bind();
	glUniform4ui(location, v0, v1, v2, v3);
}

void Shader::set_uniform_float(int location, float v0,
					     float v1,
					     float v2,
					     float v3) {
	bind();
	glUniform4f(location, v0, v1, v2, v3);
}

void Shader::set_uniform_int(const std::string& name, int v0,
						      int v1,
						      int v2,
						      int v3) {
	int loc = get_uniform_location(name);
	set_uniform_int(loc, v0, v1, v2, v3);
}

void Shader::set_uniform_uint(const std::string& name, unsigned int v0,
						       unsigned int v1,
						       unsigned int v2,
						       unsigned int v3) {
	int loc = get_uniform_location(name);
	set_uniform_uint(loc, v0, v1, v2, v3);
}

void Shader::set_uniform_float(const std::string& name, float v0,
							float v1,
							float v2,
							float v3) {
	int loc = get_uniform_location(name);
	set_uniform_float(loc, v0, v1, v2, v3);
}

void Shader::set_uniform_int(int location,
			     unsigned int count,
			     unsigned int elements,
			     const int *value) {
	if(location < 0)
		return;

	bind();

	switch(elements) {
		case 1:
			glUniform1iv(location, count, value);
			break;
		case 2:
			glUniform2iv(location, count, value);
			break;
		case 3:
			glUniform3iv(location, count, value);
			break;
		case 4:
			glUniform4iv(location, count, value);
			break;
		default:
			std::string error = "Wrong number of elements given to"
				"the set_uniform function";
			App::error_string.push_back(error);
			throw std::runtime_error(error);
			break;
	}
}

void Shader::set_uniform_uint(int location,
			      unsigned int count,
			      unsigned int elements,
			      const unsigned int *value) {
	if(location < 0)
		return;

	bind();

	switch(elements) {
		case 1:
			glUniform1uiv(location, count, value);
			break;
		case 2:
			glUniform2uiv(location, count, value);
			break;
		case 3:
			glUniform3uiv(location, count, value);
			break;
		case 4:
			glUniform4uiv(location, count, value);
			break;
		default:
			std::string error = "Wrong number of elements given to"
				"the set_uniform function";
			App::error_string.push_back(error);
			throw std::runtime_error(error);
			break;
	}
}

void Shader::set_uniform_float(int location,
			       unsigned int count,
			       unsigned int elements,
			       const float *value) {
	if(location < 0)
		return;

	bind();

	switch(elements) {
		case 1:
			glUniform1fv(location, count, value);
			break;
		case 2:
			glUniform2fv(location, count, value);
			break;
		case 3:
			glUniform3fv(location, count, value);
			break;
		case 4:
			glUniform4fv(location, count, value);
			break;
		default:
			std::string error = "Wrong number of elements given to"
				"the set_uniform function";
			App::error_string.push_back(error);
			throw std::runtime_error(error);
			break;
	}
}

void Shader::set_uniform_int(const std::string& name,
			     unsigned int count,
			     unsigned int elements,
			     const int *value) {

	int loc = get_uniform_location(name);
	set_uniform_int(loc, count, elements, value);
}

void Shader::set_uniform_uint(const std::string& name,
			      unsigned int count,
			      unsigned int elements,
			      const unsigned int *value) {

	int loc = get_uniform_location(name);
	set_uniform_uint(loc, count, elements, value);
}

void Shader::set_uniform_float(const std::string& name,
			       unsigned int count,
			       unsigned int elements,
			       const float *value) {

	int loc = get_uniform_location(name);
	set_uniform_float(loc, count, elements, value);
}

void Shader::set_uniform(int location,
			 unsigned int count,
			 unsigned int columns,
			 unsigned int rows,
			 bool transpose,
			 const float *value) {
	if(location < 0)
		return;
	if(columns < 2 || rows < 2 || columns > 4 || rows > 4)
		return;

	bind();

	if(columns == rows) {
		switch(rows) {
			case 2:
				glUniformMatrix2fv(location,
						   count,
						   transpose,
						   value);
				break;
			case 3:
				glUniformMatrix3fv(location,
						   count,
						   transpose,
						   value);
				break;
			case 4:
				glUniformMatrix4fv(location,
						   count,
						   transpose,
						   value);
				break;
		}
	} else  {
		if(columns == 2 && rows == 3) {
			glUniformMatrix2x3fv(location, count, transpose, value);
		} else if(columns == 3 && rows == 2) {
			glUniformMatrix3x2fv(location, count, transpose, value);
		} else if(columns == 2 && rows == 4) {
			glUniformMatrix2x4fv(location, count, transpose, value);
		} else if(columns == 4 && rows == 2) {
			glUniformMatrix4x2fv(location, count, transpose, value);
		} else if(columns == 3 && rows == 4) {
			glUniformMatrix3x4fv(location, count, transpose, value);
		} else if(columns == 4 && rows == 3) {
			glUniformMatrix4x3fv(location, count, transpose, value);
		}
	}
}

void Shader::set_uniform(const std::string& name,
			 unsigned int count,
			 unsigned int columns,
			 unsigned int rows,
			 bool transpose,
			 const float *value) {

	int loc = get_uniform_location(name);
	set_uniform(loc, count, columns, rows, transpose, value);
}

void Shader::set_uniform(int location, const glm::vec2& value) {
	set_uniform_float(location, value.x, value.y);
}

void Shader::set_uniform(int location, const glm::vec3& value) {
	set_uniform_float(location, value.x, value.y, value.z);
}

void Shader::set_uniform(int location, const glm::vec4& value) {
	set_uniform_float(location, value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(const std::string& name, const glm::vec2& value) {
	set_uniform_float(name, value.x, value.y);
}

void Shader::set_uniform(const std::string& name, const glm::vec3& value) {
	set_uniform_float(name, value.x, value.y, value.z);
}

void Shader::set_uniform(const std::string& name, const glm::vec4& value) {
	set_uniform_float(name, value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(int location, bool transpose, const glm::mat2& value) {
	set_uniform(location, 1, 2, 2, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(int location, bool transpose, const glm::mat3& value) {
	set_uniform(location, 1, 3, 3, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(int location, bool transpose, const glm::mat4& value) {
	set_uniform(location, 1, 4, 4, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const glm::mat2& value) {
	set_uniform(name, 1, 2, 2, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const glm::mat3& value) {
	set_uniform(name, 1, 3, 3, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const glm::mat4& value) {
	set_uniform(name, 1, 4, 4, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(int location, const std::vector<glm::vec2>& value) {
	set_uniform_float(location, value.size(), 2, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location, const std::vector<glm::vec3>& value) {
	set_uniform_float(location, value.size(), 3, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location, const std::vector<glm::vec4>& value) {
	set_uniform_float(location, value.size(), 4, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 const std::vector<glm::vec2>& value) {
	set_uniform_float(name, value.size(), 2, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 const std::vector<glm::vec3>& value) {
	set_uniform_float(name, value.size(), 3, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 const std::vector<glm::vec4>& value) {
	set_uniform_float(name, value.size(), 4, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location,
			 bool transpose,
			 const std::vector<glm::mat2>& value) {
	set_uniform(location, value.size(), 2, 2,
		    transpose, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location,
			 bool transpose,
			 const std::vector<glm::mat3>& value) {
	set_uniform(location, value.size(), 3, 3,
		    transpose, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location,
			 bool transpose,
			 const std::vector<glm::mat4>& value) {
	set_uniform(location, value.size(), 4, 4,
		    transpose, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const std::vector<glm::mat2>& value) {
	set_uniform(name, value.size(), 2, 2,
		    transpose, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const std::vector<glm::mat3>& value) {
	set_uniform(name, value.size(), 3, 3,
		    transpose, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const std::vector<glm::mat4>& value) {
	set_uniform(name, value.size(), 4, 4,
		    transpose, glm::value_ptr(value[0]));
}
