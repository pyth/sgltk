#version 150

in vec2 tc;

out vec4 color;

uniform float time;
uniform sampler2D Texture;

float rand(vec2 v) {
	return fract(sin(dot(v, vec2(42.9456, 84.850))) * 50845.8745);
}

void main() {
	vec4 tex = texture2D(Texture, tc);
	color = tex;
	//color *= vec4(tc,0,1);
	//color = vec4(rand(tc * time));
}
