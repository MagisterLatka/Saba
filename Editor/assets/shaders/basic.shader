#type vertex
#version 420 core

layout(location = 0) in vec2 i_Pos;

void main() {
    gl_Position = vec4(i_Pos, 0.0f, 1.0f);
}

#type fragment
#version 420 core

out vec4 o_Color;

void main() {
    o_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
