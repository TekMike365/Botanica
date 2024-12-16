#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;

uniform mat4 uVP;

out vec4 vCol;

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    vCol = aCol;
}