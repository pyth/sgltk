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
	transform_feedback = false;
	modify = true;
	linked = false;
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
	int info_log_length;
	std::ifstream file;
	const char *code;
	GLuint tmp;
	GLint size;

	if((filename.length() > 1 && filename[0] == '/') ||
			(filename.length() > 2 && filename[1] == ':')) {
		//absolute path
		file.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
		path = filename;
	} else {
		//relative path
		for(unsigned int i = 0; i < paths.size(); i++) {
			file.open((paths[i] + filename).c_str(), std::ifstream::in | std::ifstream::binary);
			if(file) {
				path = paths[i] + filename;
				break;
			}
		}
	}

	if(!file) {
		App::error_string.push_back(std::string("Could not open shader file: ")
				+ filename);
		return false;
	}
	file.seekg(0, file.end);
	size = static_cast<GLint>(file.tellg());
	file.seekg(0, file.beg);
	std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
	file.read(buf.get(), size);

	file.close();
	code = buf.get();

	tmp = glCreateShader(type);
	glShaderSource(tmp, 1, &code, &size);
	glCompileShader(tmp);
	glGetShaderiv(tmp, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
		glGetShaderiv(tmp, GL_INFO_LOG_LENGTH, &info_log_length);
		std::unique_ptr<char[]> info_log = std::make_unique<char[]>(info_log_length);
		glGetShaderInfoLog(tmp, info_log_length, &info_log_length, info_log.get());
		std::cerr << "CompileShader() infoLog " << filename
			  << std::endl << info_log.get() << std::endl;
		glDeleteShader(tmp);
		return false;
	}

	glAttachShader(program, tmp);
	attached.push_back(tmp);
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
	int info_log_length;

	GLuint tmp = glCreateShader(type);
	glShaderSource(tmp, 1, &(string_ptr), &(size));
	glCompileShader(tmp);
	glGetShaderiv(tmp, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
		glGetShaderiv(tmp, GL_INFO_LOG_LENGTH, &info_log_length);
		std::unique_ptr<char[]> infoLog = std::make_unique<char[]>(info_log_length);
		glGetShaderInfoLog(tmp, info_log_length, &info_log_length, infoLog.get());
		std::cerr << "CompileShader() infoLog " << std::endl
			  << infoLog.get() << std::endl;
		glDeleteShader(tmp);
		return false;
	}

	glAttachShader(program, tmp);
	attached.push_back(tmp);
	if(modify) {
		shader_string_map[shader_string] = type;
	}

	glDeleteShader(tmp);
	return true;
}

void Shader::set_transform_feedback_variables(std::vector<std::string>& variables, GLenum buffer_mode) {
	transform_feedback = true;
	std::vector<const char*> vars(variables.size());
	for(unsigned int i = 0; i < variables.size(); i++) {
		vars[i] = variables[i].c_str();
	}
	glTransformFeedbackVaryings(program, variables.size(), vars.data(), buffer_mode);
	if(linked) {
		link();
	}
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

bool Shader::link() {
	GLint isLinked = 0;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE) {
		GLint info_log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::unique_ptr<char[]> info_log = std::make_unique<char[]>(info_log_length);
		glGetProgramInfoLog(program, info_log_length, &info_log_length, info_log.get());
		App::error_string.push_back(std::string(info_log.get()));
		linked = false;
		return false;
	}
	for(GLuint shader : attached) {
		glDetachShader(program, shader);
	}
	linked = true;
	return true;
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

void Shader::set_uniform(int location, const glm::vec2& value) {
	set_uniform(location, value.x, value.y);
}

void Shader::set_uniform(int location, const glm::dvec2& value) {
	set_uniform(location, value.x, value.y);
}

void Shader::set_uniform(int location, const glm::vec3& value) {
	set_uniform(location, value.x, value.y, value.z);
}

void Shader::set_uniform(int location, const glm::dvec3& value) {
	set_uniform(location, value.x, value.y, value.z);
}

void Shader::set_uniform(int location, const glm::vec4& value) {
	set_uniform(location, value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(int location, const glm::dvec4& value) {
	set_uniform(location, value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(const std::string& name, const glm::vec2& value) {
	set_uniform(name, value.x, value.y);
}

void Shader::set_uniform(const std::string& name, const glm::dvec2& value) {
	set_uniform(name, value.x, value.y);
}

void Shader::set_uniform(const std::string& name, const glm::vec3& value) {
	set_uniform(name, value.x, value.y, value.z);
}

void Shader::set_uniform(const std::string& name, const glm::dvec3& value) {
	set_uniform(name, value.x, value.y, value.z);
}

void Shader::set_uniform(const std::string& name, const glm::vec4& value) {
	set_uniform(name, value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(const std::string& name, const glm::dvec4& value) {
	set_uniform(name, value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(int location, bool transpose, const glm::mat2& value) {
	set_uniform(location, 1, 2, 2, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(int location, bool transpose, const glm::dmat2& value) {
	set_uniform(location, 1, 2, 2, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(int location, bool transpose, const glm::mat3& value) {
	set_uniform(location, 1, 3, 3, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(int location, bool transpose, const glm::dmat3& value) {
	set_uniform(location, 1, 3, 3, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(int location, bool transpose, const glm::mat4& value) {
	set_uniform(location, 1, 4, 4, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(int location, bool transpose, const glm::dmat4& value) {
	set_uniform(location, 1, 4, 4, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const glm::mat2& value) {
	set_uniform(name, 1, 2, 2, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const glm::dmat2& value) {
	set_uniform(name, 1, 2, 2, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const glm::mat3& value) {
	set_uniform(name, 1, 3, 3, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const glm::dmat3& value) {
	set_uniform(name, 1, 3, 3, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const glm::mat4& value) {
	set_uniform(name, 1, 4, 4, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const glm::dmat4& value) {
	set_uniform(name, 1, 4, 4, transpose, glm::value_ptr(value));
}

void Shader::set_uniform(int location, const std::vector<glm::vec2>& value) {
	set_uniform(location, value.size(), 2, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location, const std::vector<glm::dvec2>& value) {
	set_uniform(location, value.size(), 2, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location, const std::vector<glm::vec3>& value) {
	set_uniform(location, value.size(), 3, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location, const std::vector<glm::dvec3>& value) {
	set_uniform(location, value.size(), 3, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location, const std::vector<glm::vec4>& value) {
	set_uniform(location, value.size(), 4, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location, const std::vector<glm::dvec4>& value) {
	set_uniform(location, value.size(), 4, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 const std::vector<glm::vec2>& value) {
	set_uniform(name, value.size(), 2, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 const std::vector<glm::dvec2>& value) {
	set_uniform(name, value.size(), 2, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 const std::vector<glm::vec3>& value) {
	set_uniform(name, value.size(), 3, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 const std::vector<glm::dvec3>& value) {
	set_uniform(name, value.size(), 3, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 const std::vector<glm::vec4>& value) {
	set_uniform(name, value.size(), 4, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 const std::vector<glm::dvec4>& value) {
	set_uniform(name, value.size(), 4, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location,
			 bool transpose,
			 const std::vector<glm::mat2>& value) {
	set_uniform(location, value.size(), 2, 2,
		    transpose, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location,
			 bool transpose,
			 const std::vector<glm::dmat2>& value) {
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
			 const std::vector<glm::dmat3>& value) {
	set_uniform(location, value.size(), 3, 3,
		    transpose, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location,
			 bool transpose,
			 const std::vector<glm::mat4>& value) {
	set_uniform(location, value.size(), 4, 4,
		    transpose, glm::value_ptr(value[0]));
}

void Shader::set_uniform(int location,
			 bool transpose,
			 const std::vector<glm::dmat4>& value) {
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
			 const std::vector<glm::dmat2>& value) {
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
			 const std::vector<glm::dmat3>& value) {
	set_uniform(name, value.size(), 3, 3,
		    transpose, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const std::vector<glm::mat4>& value) {
	set_uniform(name, value.size(), 4, 4,
		    transpose, glm::value_ptr(value[0]));
}

void Shader::set_uniform(const std::string& name,
			 bool transpose,
			 const std::vector<glm::dmat4>& value) {
	set_uniform(name, value.size(), 4, 4,
		    transpose, glm::value_ptr(value[0]));
}
