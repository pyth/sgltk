#version 130

in vec4 pos_eye;
in vec3 tc;

out vec4 color;

uniform sampler2D Texture;

void main() {
	vec4 tex = texture(Texture, tc.xy);
	color = tex;
}
