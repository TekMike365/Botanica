#version 430 core

layout (location = 0) in vec4 aPos;

uniform mat4 uVP;

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}