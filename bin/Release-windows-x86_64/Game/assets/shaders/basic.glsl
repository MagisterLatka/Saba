#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec2 i_UV;

out vec2 l_UV;

uniform mat4 u_ViewProjMat = mat4(1.0f);

void main()
{
	gl_Position = u_ViewProjMat * vec4(i_Pos, 1.0f);
	l_UV = i_UV;
}

#type fragment
#version 330 core

out vec4 o_Color;

uniform sampler2D u_Tex;

in vec2 l_UV;

void main()
{
	o_Color = texture(u_Tex, l_UV);
}
