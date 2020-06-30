#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 5) in mat4 i_ModelMat;

void main()
{
	gl_Position = i_ModelMat * vec4(i_Pos, 1.0f);
}

#type geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 u_LightSpace[6];

out vec4 l_Pos;

void main()
{
	for (int face = 0; face < 6; face++)
	{
		gl_Layer = face;
		for (int i = 0; i < 3; i++)
		{
			l_Pos = gl_in[i].gl_Position;
			gl_Position = u_LightSpace[face] * l_Pos;
			EmitVertex();
		}
		EndPrimitive();
	}
}

#type fragment
#version 330 core

in vec4 l_Pos;

uniform vec3 u_LightPos;
uniform float u_FarPlane;

void main()
{
	gl_FragDepth = length(l_Pos.xyz - u_LightPos) / u_FarPlane;
}
