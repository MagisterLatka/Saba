#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;

uniform mat4 u_ViewProjMat;

void main()
{
	gl_Position = u_ViewProjMat * vec4(i_Pos, 1.0f);
}

#type fragment
#version 330 core

out vec4 o_Color;

void main()
{
	o_Color = vec4(0.2f, 0.3f, 0.8f, 1.0f);
}
