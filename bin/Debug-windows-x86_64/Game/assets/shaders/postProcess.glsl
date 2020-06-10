#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec2 i_UV;

out vec2 l_UV;

void main()
{
	gl_Position = vec4(i_Pos, 1.0f);
	l_UV = i_UV;
}

#type fragment
#version 330 core

out vec4 o_Color;

in vec2 l_UV;

uniform sampler2D u_Tex;
uniform float u_Exposure;

void main()
{
	vec4 color = texture(u_Tex, l_UV);

	color.xyw = vec3(1.0f) - exp(-color.xyz * u_Exposure);
	color.xyz = pow(color.xyz, vec3(1.0f / 2.2f));

	o_Color = color;
}
