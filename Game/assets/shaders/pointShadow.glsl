#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 4) in mat4 i_ModelMat;

void main()
{
	gl_Position = i_ModelMat * vec4(i_Pos, 1.0f);
}

#type geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

uniform mat4 u_LightSpace[2];
uniform float u_FarPlane;

out float l_Depth;

void main()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			gl_Position = gl_in[j].gl_Position;
			gl_Position = u_LightSpace[i] * gl_Position;

			float distance = -gl_Position.z;
			l_Depth = length(gl_Position.xyz);

			gl_Position.xyz = normalize(gl_Position.xyz);
			gl_Position.xy /= 1.0f - gl_Position.z;
			gl_Position.z = (distance / u_FarPlane) * 2.0f - 1.0f;
			gl_Position.w = 1.0f;

			gl_Position.x = gl_Position.x * 0.5f - 0.5f + float(i);

			EmitVertex();
		}
		EndPrimitive();
	}
}

#type fragment
#version 330 core

in float l_Depth;

uniform float u_FarPlane;

void main()
{
	gl_FragDepth = l_Depth / u_FarPlane;
}
