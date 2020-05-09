#type vertex
#version 420 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec2 i_UV;
layout(location = 3) in vec4 i_Color;
layout(location = 4) in float i_TID;
layout(location = 5) in mat4 i_ModelMat;
layout(location = 9) in vec4 i_MulColor;
layout(location = 10) in float i_TIDoptional;

layout(std140, binding = 0) uniform ViewProjMat
{
	mat4 u_ViewProjMat;
};

out DATA {
	vec3 pos;
	vec3 normal;
	vec2 uv;
	vec4 color;
	float tid;
	vec4 mulColor;
	float tidop;
} vs_out;

void main()
{
	gl_Position = u_ViewProjMat * i_ModelMat * vec4(i_Pos, 1.0f);
	vs_out.pos = vec3(i_ModelMat * vec4(i_Pos, 1.0f));
	vs_out.normal = normalize(mat3(transpose(inverse(i_ModelMat))) * i_Normal);
	vs_out.uv = i_UV;
	vs_out.color = i_Color;
	vs_out.tid = i_TID;
	vs_out.mulColor = i_MulColor;
	vs_out.tidop = i_TIDoptional;
}

#type fragment
#version 420 core

out vec4 o_Color;

in DATA {
	vec3 pos;
	vec3 normal;
	vec2 uv;
	vec4 color;
	float tid;
	vec4 mulColor;
	float tidop;
} fs_in;

struct Light
{
	vec4 pos;
	vec4 dir;
	vec3 diffuse;
	vec3 specular;
	vec2 cutOffs;

	vec3 attenuation;
};
const int c_MaxLights = 10;
layout(std140, binding = 1) uniform Scene {
	vec3 u_ViewPos;
	Light u_Lights[c_MaxLights];
};

const float c_Ambient = 0.1f;

uniform sampler2D u_Tex[32];

vec3 CalcLight(Light light, vec3 pos, vec3 normal, vec3 color, vec3 viewPos);

void main()
{
	vec4 color = texture(u_Tex[int(fs_in.tid + fs_in.tidop + 0.1f)], fs_in.uv) * fs_in.color * fs_in.mulColor;
	vec3 outputColor = color.rgb * c_Ambient;
	for (int i = 0; i < c_MaxLights; i++)
		outputColor += CalcLight(u_Lights[i], fs_in.pos, fs_in.normal, color.rgb, u_ViewPos);
	o_Color = vec4(outputColor, color.a);
}

vec3 CalcLight(Light light, vec3 pos, vec3 normal, vec3 color, vec3 viewPos)
{
	const float shininess = 8.0f; //TODO: material system
	const vec3 materialSpec = vec3(1.0f);
	//dir light
	if (light.pos.w == 0.0f && light.dir.w == 1.0f)
	{
		vec3 toLight = normalize(-light.dir.xyz);
		float diff = max(dot(toLight, normal), 0.0f);
		vec3 diffuse = color * light.diffuse * diff;

		vec3 toView = normalize(viewPos - pos);
		vec3 halfwayDir = normalize(toLight + toView);
		float spec = pow(max(dot(halfwayDir, normal), 0.0f), shininess);
		vec3 specular = materialSpec * light.specular * spec;

		return diffuse + specular;
	}
	//point light
	else if (light.pos.w == 1.0f && light.dir.w == 0.0f)
	{
		vec3 toLight = normalize(light.pos.xyz - pos);
		float diff = max(dot(toLight, normal), 0.0f);
		vec3 diffuse = color * light.diffuse * diff;

		vec3 toView = normalize(viewPos - pos);
		vec3 halfwayDir = normalize(toLight + toView);
		float spec = pow(max(dot(halfwayDir, normal), 0.0f), shininess);
		vec3 specular = materialSpec * light.specular * spec;

		float distance = length(toLight);
		float attenuation = 1.0f / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * distance * distance);

		diffuse *= attenuation;
		specular *= attenuation;

		return diffuse + specular;
	}
	//spot light
	else if (light.pos.w == 1.0f && light.dir.w == 1.0f)
	{
		vec3 toLight = normalize(light.pos.xyz - pos);
		float diff = max(dot(toLight, normal), 0.0f);
		vec3 diffuse = color * light.diffuse * diff;

		vec3 toView = normalize(viewPos - pos);
		vec3 halfwayDir = normalize(toLight + toView);
		float spec = pow(max(dot(halfwayDir, normal), 0.0f), shininess);
		vec3 specular = materialSpec * light.specular * spec;

		float distance = length(toLight);
		float attenuation = 1.0f / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * distance * distance);

		diffuse *= attenuation;
		specular *= attenuation;

		float theta = dot(toLight, normalize(-light.dir.xyz));
		float epsilon = light.cutOffs.x - light.cutOffs.y;
		float intensity = clamp((theta - light.cutOffs.y) / epsilon, 0.0f, 1.0f);
		diffuse *= intensity;
		specular *= intensity;

		return diffuse + specular;
	}
	return vec3(0.0f);
}
