#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 4) in mat4 i_ModelMat;

out float l_Depth;

uniform mat4 u_LightSpace;
uniform float u_FarPlane;
uniform float u_CutOff;

void main()
{
	gl_Position = u_LightSpace * i_ModelMat * vec4(i_Pos, 1.0f);
	
	l_Depth = length(gl_Position.xyz);

	gl_Position.w = -gl_Position.z;
	gl_Position.xy /= tan(u_CutOff);
	gl_Position.z = (-gl_Position.z / u_FarPlane) * 2.0f - 1.0f;
}

#type fragment
#version 330 core

in float l_Depth;

uniform float u_FarPlane;

void main()
{
	gl_FragDepth = l_Depth / u_FarPlane;
}
