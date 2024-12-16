#pragma once

inline const char *Vertex_glsl = R"(
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;

uniform mat4 uVP;

out vec4 vCol;

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    vCol = aCol;
})";

inline const char *Fragment_glsl = R"(
#version 430 core

in vec4 vCol;

out vec4 fColor;

void main()
{
    fColor = vCol;
})";

