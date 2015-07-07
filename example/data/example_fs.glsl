#version 150

in vec4 pos_eye;
in vec3 norm_eye;

in vec4 col;
in vec3 tc;

out vec4 color;

uniform float time;
uniform vec3 light_dir;
uniform sampler2D Texture;

float rand(vec2 v) {
	return fract(sin(dot(v, vec2(42.9456, 84.850))) * 50845.8745);
}

void main() {
	vec4 tex = texture(Texture, tc.xy);

	vec3 camera = normalize(-pos_eye.xyz);
	vec3 norm = normalize(norm_eye);
	vec3 light = -normalize(light_dir);
	vec3 reflection = -normalize(reflect(light, norm));

	float VR = max(0.0, dot(camera, reflection));
	float LN = max(0.0, dot(norm, light));

	vec4 amb = vec4(0.2 * col.xyz, col.w);
	vec4 diff = vec4(LN * col.xyz, col.w);
	vec4 spec = vec4(1,1,1,0) * 0.5 * pow(VR, 20.0);

	color = max(amb, diff) + spec;
}
