#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 5) in mat4 i_ModelMat;

uniform mat4 u_LightSpace;

void main()
{
	gl_Position = u_LightSpace * i_ModelMat * vec4(i_Pos, 1.0f);
}

#type fragment
#version 330 core

void main()
{
	
}
