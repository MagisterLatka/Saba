#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec2 i_UV;
layout(location = 2) in vec4 i_Color;
layout(location = 3) in float i_TID;
layout(location = 4) in mat4 i_ModelMat;
layout(location = 8) in vec4 i_MulColor;
layout(location = 9) in float i_TIDoptional;

out DATA {
	vec2 uv;
	vec4 color;
	float tid;
	vec4 mulColor;
	float tidop;
} vs_out;

uniform mat4 u_ViewProjMat;

void main()
{
	gl_Position = u_ViewProjMat * i_ModelMat * vec4(i_Pos, 1.0f);
	vs_out.uv = i_UV;
	vs_out.color = i_Color;
	vs_out.tid = i_TID;
	vs_out.mulColor = i_MulColor;
	vs_out.tidop = i_TIDoptional;
}

#type fragment
#version 330 core

out vec4 o_Color;

in DATA {
	vec2 uv;
	vec4 color;
	float tid;
	vec4 mulColor;
	float tidop;
} fs_in;

uniform sampler2D u_Tex[32];

void main()
{
	o_Color = texture(u_Tex[int(fs_in.tid + fs_in.tidop + 0.1f)], fs_in.uv) * fs_in.color * fs_in.mulColor;
}

