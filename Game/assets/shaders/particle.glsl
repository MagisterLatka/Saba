#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_ViewProjMat * u_Transform * vec4(i_Pos, 1.0f);
}


#type fragment
#version 330 core

out vec4 o_Color;

uniform vec4 u_Color;

void main()
{
	o_Color = u_Color;
}
