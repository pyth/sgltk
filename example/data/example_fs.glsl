#version 150

in vec3 position;
in vec3 norm_eye;
in vec2 tc;

out vec4 color;

uniform float time;
uniform vec3 light_pos;
uniform sampler2D Texture;

float rand(vec2 v) {
	return fract(sin(dot(v, vec2(42.9456, 84.850))) * 50845.8745);
}

void main() {
	vec4 tex = texture(Texture, tc);
	color = vec4(1.0);
	color = vec4(norm_eye, 1.0);
	color = tex;
}
