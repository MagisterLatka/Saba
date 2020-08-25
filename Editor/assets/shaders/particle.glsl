#type vertex
#version 330 core

layout(location = 0) in vec4 i_Pos;
layout(location = 1) in vec4 i_Color;

uniform mat4 u_ViewProjMat;

out vec4 l_Color;

void main()
{
	l_Color = i_Color;
	gl_Position = u_ViewProjMat * i_Pos;
}


#type fragment
#version 330 core

out vec4 o_Color;

in vec4 l_Color;

void main()
{
	o_Color = l_Color;
}
