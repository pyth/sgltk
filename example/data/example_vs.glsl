#version 150

in vec4 pos;
in vec3 norm;
in vec3 tc_in;
in vec4 col_in;

out vec4 pos_eye;
out vec3 norm_eye;

out vec4 col;
out vec3 tc;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 NM;

void main() {
	pos_eye = MV * pos;
	norm_eye = NM * norm;

	col = col_in;
	tc = tc_in;

	gl_Position = MVP * pos;
}
