#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec4 i_Color;

uniform mat4 u_ViewProjMat;

out vec4 u_Color;

void main()
{
	gl_Position = u_ViewProjMat * vec4(i_Pos, 1.0f);
	u_Color = i_Color;
}


#type fragment
#version 330 core

out vec4 o_Color;

in vec4 u_Color;

void main()
{
	o_Color = u_Color;
}
