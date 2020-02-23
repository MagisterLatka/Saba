#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in mat4 i_Transform;
layout(location = 2) in vec4 i_Color;

uniform mat4 u_ViewProjMat;

out vec4 l_Color;

void main()
{
	gl_Position = u_ViewProjMat * i_Transform * vec4(i_Pos, 1.0f);
	l_Color = i_Color;
}


#type fragment
#version 330 core

out vec4 o_Color;

in vec4 l_Color;

void main()
{
	o_Color = l_Color;
}
