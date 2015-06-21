#version 150

in vec4 pos;
in vec4 norm;
in vec2 tc_in;

out vec2 tc;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 NormalMatrix;
uniform vec4 LigthSource;

void main() {
	gl_Position = MVP * pos;
	tc = tc_in;
}
