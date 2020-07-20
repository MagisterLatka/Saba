#type vertex
#version 330 core

layout(location = 0) in vec3 i_Pos;

out vec3 l_UV;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
	l_UV = i_Pos;
	gl_Position = (u_ProjMat * u_ViewMat * vec4(i_Pos, 1.0f)).xyww;
}

#type fragment
#version 420 core

out vec4 o_Color;

in vec3 l_UV;

uniform samplerCube u_Tex;

void main()
{
	o_Color = texture(u_Tex, l_UV);
}
