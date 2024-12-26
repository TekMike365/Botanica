#version 430 core

layout (location = 0) in vec4 aPos;

uniform mat4 uVP;

out vec4 vColor;

vec4 g_Colors[] = {
    vec4(0.0, 0.0, 0.0, 1.0),
    vec4(0.0, 0.0, 1.0, 1.0),
    vec4(0.0, 1.0, 0.0, 1.0),
    vec4(0.0, 1.0, 1.0, 1.0),
    vec4(1.0, 0.0, 0.0, 1.0),
    vec4(1.0, 0.0, 1.0, 1.0),
    vec4(1.0, 1.0, 0.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
};

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    vColor = g_Colors[uint(aPos.w)];
}