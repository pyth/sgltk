#version 150

in vec4 pos;
in vec3 norm;
in vec2 tc_in;

out vec3 norm_eye;
out vec2 tc;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 NM;

void main() {
	gl_Position = MVP * pos;
	norm_eye = norm;
	tc = tc_in;
}
