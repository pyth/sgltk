#version 130

struct Matrix {
	mat4 model_view;
	mat4 model_view_proj;
	mat3 normal_matrix;
};

in vec4 pos_in;
in vec3 norm_in;
in vec3 tex_coord_in0;

out vec4 pos_eye;
out vec3 norm_eye;
out vec3 tc;

uniform Matrix matrix;
uniform sampler2D Texture;

void main() {
	pos_eye = matrix.model_view * pos_in;
	norm_eye = matrix.normal_matrix * norm_in;

	tc = tex_coord_in0;

	gl_Position = matrix.model_view_proj * pos_in;
}
