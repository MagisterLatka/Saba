#type vertex
#version 420 core

layout(location = 0) in vec4 i_Pos_IsLighted;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec2 i_UV;
layout(location = 3) in vec4 i_Color;
layout(location = 4) in float i_TID;
layout(location = 5) in mat4 i_ModelMat;
layout(location = 9) in vec4 i_MulColor;
layout(location = 10) in vec2 i_TIDoptional_IsLighted;

layout(std140, binding = 0) uniform ViewProjMat
{
	mat4 u_ViewProjMat;
};

const int c_MaxLights = 10;
out DATA {
	vec3 pos;
	vec3 normal;
	vec2 uv;
	vec4 color;
	float tid;
	vec4 mulColor;
	float tidop;

	float isLighted;

	vec3 posInDirLightSpace[c_MaxLights];
} vs_out;

struct Light
{
	vec4 pos;
	vec4 dir;
	vec4 diffuse;
	vec4 specular;
	vec4 cutOffs_FarPlane;

	vec4 attenuation;

	vec4 shadowTextureSpace;
	mat4 dirLightSpace;
};
layout(std140, binding = 1) uniform Scene {
	Light u_Lights[c_MaxLights];
	vec4 u_ViewPos;
	ivec4 u_ShadowTexIndexes;
};

void main()
{
	gl_Position = u_ViewProjMat * i_ModelMat * vec4(i_Pos_IsLighted.xyz, 1.0f);
	vs_out.pos = vec3(i_ModelMat * vec4(i_Pos_IsLighted.xyz, 1.0f));
	vs_out.normal = normalize(mat3(transpose(inverse(i_ModelMat))) * i_Normal);
	vs_out.uv = i_UV;
	vs_out.color = i_Color;
	vs_out.tid = i_TID;
	vs_out.mulColor = i_MulColor;
	vs_out.tidop = i_TIDoptional_IsLighted.x;
	vs_out.isLighted = i_Pos_IsLighted.w * i_TIDoptional_IsLighted.y;
	for (int i = 0; i < c_MaxLights; i++)
		vs_out.posInDirLightSpace[i] = vec3(u_Lights[i].dirLightSpace * vec4(vs_out.pos, 1.0f));
}

#type fragment
#version 420 core

float equal(float x, float y); float not_equal(float x, float y); float greater(float x, float y); float less(float x, float y); float greater_equal(float x, float y); float less_equal(float x, float y);
float and(float a, float b); float or(float a, float b); float not(float a);
vec2 equal(vec2 x, vec2 y); vec2 not_equal(vec2 x, vec2 y); vec2 greater(vec2 x, vec2 y); vec2 less(vec2 x, vec2 y); vec2 greater_equal(vec2 x, vec2 y); vec2 less_equal(vec2 x, vec2 y);
vec2 and(vec2 a, vec2 b); vec2 or(vec2 a, vec2 b); vec2 not(vec2 a);
vec3 equal(vec3 x, vec3 y); vec3 not_equal(vec3 x, vec3 y); vec3 greater(vec3 x, vec3 y); vec3 less(vec3 x, vec3 y); vec3 greater_equal(vec3 x, vec3 y); vec3 less_equal(vec3 x, vec3 y);
vec3 and(vec3 a, vec3 b); vec3 or(vec3 a, vec3 b); vec3 not(vec3 a);
vec4 equal(vec4 x, vec4 y); vec4 not_equal(vec4 x, vec4 y); vec4 greater(vec4 x, vec4 y); vec4 less(vec4 x, vec4 y); vec4 greater_equal(vec4 x, vec4 y); vec4 less_equal(vec4 x, vec4 y);
vec4 and(vec4 a, vec4 b); vec4 or(vec4 a, vec4 b); vec4 not(vec4 a);


out vec4 o_Color;


const int c_MaxLights = 10;
in DATA {
	vec3 pos;
	vec3 normal;
	vec2 uv;
	vec4 color;
	float tid;
	vec4 mulColor;
	float tidop;

	float isLighted;

	vec3 posInDirLightSpace[c_MaxLights];
} fs_in;

////////////////////////////////
struct Light
{
	vec4 pos;
	vec4 dir;
	vec4 diffuse;
	vec4 specular;
	vec4 cutOffs_FarPlane;

	vec4 attenuation;

	vec4 shadowTextureSpace;
	mat4 dirLightSpace;
};
layout(std140, binding = 1) uniform Scene {
	Light u_Lights[c_MaxLights];
	vec4 u_ViewPos;
	ivec4 u_ShadowTexIndexes;
};

uniform sampler2D u_Tex[32];

////////////////////////////////
const float c_Ambient = 0.05f;

////////////////////////////////
float CalcDirShadow(const vec3 pos, const float toLightNormal, const vec4 shadowTextureSpace);
float CalcPointShadow(const vec3 lightToPos, const float toLightNormal, const vec4 shadowTextureSpace, const float farPlane);

vec3 CalcLight(const Light light, const vec3 pos, const vec3 posInDirLightSpace, const vec3 normal, const vec3 color, const vec3 viewPos);

////////////////////////////////
void main()
{
	const vec4 color = texture(u_Tex[int(fs_in.tid + fs_in.tidop + 0.1f)], fs_in.uv) * fs_in.color * fs_in.mulColor;

	vec3 outputColor = color.rgb * c_Ambient;
	for (int i = 0; i < c_MaxLights; i++)
		outputColor += CalcLight(u_Lights[i], fs_in.pos, fs_in.posInDirLightSpace[i], fs_in.normal, color.rgb, u_ViewPos.xyz);

	o_Color = vec4(color.rgb * equal(fs_in.isLighted, 0.0f) + outputColor * not_equal(fs_in.isLighted, 0.0f), color.a);
}


////////////////////////////////
	const int pcfLevel = 1;
float CalcDirShadow(const vec3 pos, const float toLightNormal, const vec4 shadowTextureSpace)
{
	const vec3 position = pos * 0.5f + 0.5f;
	const vec2 uv = mix(shadowTextureSpace.xy, shadowTextureSpace.zw, position.xy);
	const float bias = max(0.05f * (1.0f - toLightNormal), 0.005f);
	
	float shadow = 0.0f;
	const vec2 texelSize = 1.0f / textureSize(u_Tex[u_ShadowTexIndexes.x], 0);
	for (int y = -pcfLevel; y <= pcfLevel; y++)
	{
		for (int x = -pcfLevel; x <= pcfLevel; x++)
		{
			const float pcfDepth = texture(u_Tex[u_ShadowTexIndexes.x], uv + vec2(x, y) * texelSize).r;
			shadow += greater(position.z - bias, pcfDepth) / ((2 * pcfLevel + 1) * (2 * pcfLevel + 1));
		}
	}

	return shadow * and(and(greater(uv.x, shadowTextureSpace.x), less(uv.x, shadowTextureSpace.z)), and(greater(uv.y, shadowTextureSpace.y), less(uv.y, shadowTextureSpace.w))) * less(position.z, 1.0f);
}
float CalcPointShadow(const vec3 lightToPos, const float toLightNormal, const vec4 shadowTextureSpace, const float farPlane)
{
	const float current = length(lightToPos);
	const float bias = max(0.4f * (1.0f - toLightNormal), 0.05f);

	const vec3 ltp = normalize(lightToPos);
	vec2 uv = (ltp.xy / (abs(ltp.z) + 1.005f)) * 0.5f + 0.5f;
	uv.x *= 0.5f;
	uv.x = uv.x * less(ltp.z, 0.0f) + (1.0f - uv.x) * greater_equal(ltp.z, 0.0f);
	uv = mix(shadowTextureSpace.xy, shadowTextureSpace.zw, uv);

	float shadow = 0.0f;
	const vec2 texelSize = 1.0f / textureSize(u_Tex[u_ShadowTexIndexes.y], 0);
	for (int y = -pcfLevel; y <= pcfLevel; y++)
	{
		for (int x = -pcfLevel; x <= pcfLevel; x++)
		{
			const float pcfDepth = texture(u_Tex[u_ShadowTexIndexes.y], uv + vec2(x, y) * texelSize).r * farPlane;
			shadow += greater(current - bias, pcfDepth) / ((2 * pcfLevel + 1) * (2 * pcfLevel + 1));
		}
	}

	return shadow * and(and(greater(uv.x, shadowTextureSpace.x), less(uv.x, shadowTextureSpace.z)), and(greater(uv.y, shadowTextureSpace.y), less(uv.y, shadowTextureSpace.w)));
}

vec3 CalcLight(const Light light, const vec3 pos, const vec3 posInDirLightSpace, const vec3 normal, const vec3 color, const vec3 viewPos)
{
	const float shininess = 32.0f; //TODO: material system
	const vec3 materialSpec = vec3(1.0f);

	const vec3 toLight = normalize(-light.dir.xyz) * and(equal(light.pos.w, 0.0f), equal(light.dir.w, 1.0f)) + normalize(light.pos.xyz - pos) * equal(light.pos.w, 1.0f);

	const float distance = length(toLight);
	const float attenuation = 1.0f / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * distance * distance);

	const float intensity = clamp((dot(toLight, normalize(-light.dir.xyz)) - light.cutOffs_FarPlane.y) / (light.cutOffs_FarPlane.x - light.cutOffs_FarPlane.y), 0.0f, 1.0f) *
								greater(light.cutOffs_FarPlane.x, 0.0f) + equal(light.cutOffs_FarPlane.x, 0.0f);

	const float dTLN = dot(toLight, normal);
	const vec3 diffuse = color * light.diffuse.rgb * max(dTLN, 0.0f) * attenuation * intensity;

	const vec3 halfwayDir = normalize(toLight + normalize(viewPos - pos));
	const float spec = pow(max(dot(halfwayDir, normal), 0.0f), shininess);
	const vec3 specular = materialSpec * light.specular.rgb * spec * attenuation * intensity;

	const float shadow = CalcDirShadow(posInDirLightSpace, dTLN, light.shadowTextureSpace) * and(equal(light.pos.w, 0.0f), equal(light.dir.w, 1.0f)) +
				CalcPointShadow(pos - light.pos.xyz, dTLN, light.shadowTextureSpace, light.cutOffs_FarPlane.z) * and(equal(light.pos.w, 1.0f), equal(light.dir.w, 0.0f));
	return max((diffuse + specular) * (1.0f - shadow), 0.0f);
}


////////////////////////////////
float equal(float x, float y) { return 1.0f - abs(sign(x - y)); } float not_equal(float x, float y) { return abs(sign(x - y)); } float greater(float x, float y) { return max(sign(x - y), 0.0f); }
float less(float x, float y) { return max(sign(y - x), 0.0f); } float greater_equal(float x, float y) { return 1.0f - less(x, y); } float less_equal(float x, float y) { return 1.0f - greater(x, y); }
float and(float a, float b) { return a * b; } float or(float a, float b) { return min(a + b, 1.0f); } float not(float a) { return 1.0f - a; }

vec2 equal(vec2 x, vec2 y) { return 1.0f - abs(sign(x - y)); } vec2 not_equal(vec2 x, vec2 y) { return abs(sign(x - y)); } vec2 greater(vec2 x, vec2 y) { return max(sign(x - y), 0.0f); }
vec2 less(vec2 x, vec2 y) { return max(sign(y - x), 0.0f); } vec2 greater_equal(vec2 x, vec2 y) { return 1.0f - less(x, y); } vec2 less_equal(vec2 x, vec2 y) { return 1.0f - greater(x, y); }
vec2 and(vec2 a, vec2 b) { return a * b; } vec2 or(vec2 a, vec2 b) { return min(a + b, 1.0f); } vec2 not(vec2 a) { return 1.0f - a; }

vec3 equal(vec3 x, vec3 y) { return 1.0f - abs(sign(x - y)); } vec3 not_equal(vec3 x, vec3 y) { return abs(sign(x - y)); } vec3 greater(vec3 x, vec3 y) { return max(sign(x - y), 0.0f); }
vec3 less(vec3 x, vec3 y) { return max(sign(y - x), 0.0f); } vec3 greater_equal(vec3 x, vec3 y) { return 1.0f - less(x, y); } vec3 less_equal(vec3 x, vec3 y) { return 1.0f - greater(x, y); }
vec3 and(vec3 a, vec3 b) { return a * b; } vec3 or(vec3 a, vec3 b) { return min(a + b, 1.0f); } vec3 not(vec3 a) { return 1.0f - a; }

vec4 equal(vec4 x, vec4 y) { return 1.0f - abs(sign(x - y)); } vec4 not_equal(vec4 x, vec4 y) { return abs(sign(x - y)); } vec4 greater(vec4 x, vec4 y) { return max(sign(x - y), 0.0f); }
vec4 less(vec4 x, vec4 y) { return max(sign(y - x), 0.0f); } vec4 greater_equal(vec4 x, vec4 y) { return 1.0f - less(x, y); } vec4 less_equal(vec4 x, vec4 y) { return 1.0f - greater(x, y); }
vec4 and(vec4 a, vec4 b) { return a * b; } vec4 or(vec4 a, vec4 b) { return min(a + b, 1.0f); } vec4 not(vec4 a) { return 1.0f - a; }
