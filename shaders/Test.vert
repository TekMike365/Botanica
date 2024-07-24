#version 430 core

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) out vec3 fColor;

void main()
{
    fColor = iColor;
    gl_Position = projection * view * model * vec4(iPosition, 1.0);
}
