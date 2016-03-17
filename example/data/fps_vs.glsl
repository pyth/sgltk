#version 130

struct Matrix {
	mat4 model_view;
	mat4 model_view_proj;
	mat3 normal_matrix;
};

in vec4 pos_in;
in vec3 tex_coord_in0;

out vec3 tc;

uniform Matrix matrix;
uniform sampler2D Texture;

void main() {
	vec4 pos = matrix.model_view_proj * pos_in;

	tc = tex_coord_in0;

	gl_Position = pos;
}
