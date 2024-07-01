#version 430 core

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iColor;

layout(location = 0) out vec3 fColor;

void main()
{
    fColor = iColor;
    gl_Position = vec4(iPosition, 1.0);
}
