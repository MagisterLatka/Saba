#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec4 i_UV_TID_TillingFactor;
layout(location = 2) in vec4 i_Color;

out DATA {
	vec2 uv;
	vec4 color;
	float tid;
	float tillingFactor;
} vs_out;

uniform mat4 u_ViewProjMat;

void main()
{
	gl_Position = u_ViewProjMat * vec4(i_Pos, 1.0f);

	vs_out.uv = i_UV_TID_TillingFactor.xy;
	vs_out.color = i_Color;
	vs_out.tid = i_UV_TID_TillingFactor.z;
	vs_out.tillingFactor = i_UV_TID_TillingFactor.w;
}


#type fragment
#version 330 core

out vec4 o_Color;

in DATA {
	vec2 uv;
	vec4 color;
	float tid;
	float tillingFactor;
} fs_in;

uniform sampler2D u_Textures[32];

void main()
{
	o_Color = texture(u_Textures[int(fs_in.tid)], fs_in.uv * fs_in.tillingFactor) * fs_in.color;
}
