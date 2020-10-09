#type vertex
#version 420 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec4 i_Normal_Use;
layout(location = 2) in vec4 i_Tangent_Use;
layout(location = 3) in vec2 i_UV;
layout(location = 4) in vec4 i_Color;

out DATA {
	vec2 uv;
	vec4 color;
} vs_out;

uniform mat4 u_ViewProjMat;
uniform mat4 u_ModelMat = mat4(1.0f);

void main()
{
	gl_Position = u_ViewProjMat * u_ModelMat * vec4(i_Pos, 1.0f);
	vs_out.uv = i_UV;
	vs_out.color = i_Color;
}

#type fragment
#version 420 core

out vec4 o_Color;

in DATA {
	vec2 uv;
	vec4 color;
} fs_in;

layout(binding = 0) uniform sampler2D u_AlbedoTex;
layout(binding = 1) uniform sampler2D u_NormalTex;
layout(binding = 2) uniform sampler2D u_HeightTex;
layout(binding = 3) uniform sampler2D u_RoughnessTex;
layout(binding = 4) uniform sampler2D u_MetallicTex;
layout(binding = 5) uniform sampler2D u_AOTex;

void main()
{
	o_Color = texture(u_AlbedoTex, fs_in.uv) * fs_in.color;
}
