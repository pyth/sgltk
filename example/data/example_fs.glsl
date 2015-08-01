#version 130

struct Material {
	vec4 color_ambient;
	vec4 color_diffuse;
	vec4 color_specular;
	float shininess;
	float shininess_strength;
};

in vec4 pos_eye;
in vec3 norm_eye;

in vec3 tc;

out vec4 color;

uniform float time;
uniform vec3 light_dir;
uniform sampler2D Texture;
uniform Material material;

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

	vec4 amb;
	vec4 diff;
	vec4 spec;

	if(material.color_ambient != vec4(0,0,0,1))
		amb = vec4(0.2 * material.color_ambient.xyz,
				material.color_ambient.w);
	else
		amb = vec4(0.2 * material.color_diffuse.xyz,
				material.color_diffuse.w);

	diff = vec4(LN * material.color_diffuse.xyz,
			material.color_diffuse.w);

	spec = material.color_specular *
			material.shininess_strength *
			pow(VR, material.shininess);

	color = amb + diff + spec;
	//color = vec4(1);
	//color = tex;
	//color = vec4(tc.xy, 0, 1);
}
