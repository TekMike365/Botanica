#pragma once

inline const char *Vertex_glsl = R"(
#version 430 core

layout (location = 0) in vec4 aPos;

uniform mat4 uVP;

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
})";

inline const char *Fragment_glsl = R"(
#version 430 core

out vec4 fColor;

void main()
{
    fColor = vec4(0.0, 0.0, 0.0, 1.0);
})";

inline const char *VoxelGen_glsl = R"(
#version 430 core
layout (local_size_x = 32, local_size_y = 1, local_size_z = 1) in;

layout (std430, binding = 1) buffer ssboVoxels { uint ib_VoxelIDs[]; };
layout (std430, binding = 2) buffer ssboVertices { vec4 ib_Vertices[]; };

uniform uvec3 uVoxelsSize;

uvec3 GetGlobalPosition()
{
    uint idx = gl_LocalInvocationIndex;
    uint width = uVoxelsSize.x;
    uint height = uVoxelsSize.y;
    uint z = idx / (width * height);
    uint y = (idx - z * width * height) / width;
    uint x = idx - width * (y + height * z);
    return uvec3(x, y, z);
}

vec4 g_Vertices[] = {
    vec4(-0.5, -0.5, -0.5, 0.0),
    vec4(0.5, -0.5, -0.5, 0.0),
    vec4(0.5, -0.5, 0.5, 0.0),
    vec4(-0.5, -0.5, 0.5, 0.0),

    vec4(-0.5, 0.5, -0.5, 0.0),
    vec4(0.5, 0.5, -0.5, 0.0),
    vec4(0.5, 0.5, 0.5, 0.0),
    vec4(-0.5, 0.5, 0.5, 0.0),
};

void main()
{
    if (gl_LocalInvocationIndex > uVoxelsSize.x * uVoxelsSize.y * uVoxelsSize.z) 
        return;

    float scale = 1.0 / 8.0;
    vec4 pos = vec4(GetGlobalPosition(), 0.0);
    uint idx = gl_LocalInvocationIndex * 24;

    // bottom
    ib_Vertices[idx + 0] = scale * (g_Vertices[0] + pos);
    ib_Vertices[idx + 1] = scale * (g_Vertices[1] + pos);
    ib_Vertices[idx + 2] = scale * (g_Vertices[2] + pos);
    ib_Vertices[idx + 3] = scale * (g_Vertices[3] + pos);

    // top
    ib_Vertices[idx + 4] = scale * (g_Vertices[4] + pos);
    ib_Vertices[idx + 5] = scale * (g_Vertices[5] + pos);
    ib_Vertices[idx + 6] = scale * (g_Vertices[6] + pos);
    ib_Vertices[idx + 7] = scale * (g_Vertices[7] + pos);

    // front
    ib_Vertices[idx + 8]= scale * (g_Vertices[0] + pos);
    ib_Vertices[idx + 9]= scale * (g_Vertices[1] + pos);
    ib_Vertices[idx + 10] = scale * (g_Vertices[5] + pos);
    ib_Vertices[idx + 11] = scale * (g_Vertices[4] + pos);

    // back
    ib_Vertices[idx + 12] = scale * (g_Vertices[2] + pos);
    ib_Vertices[idx + 13] = scale * (g_Vertices[3] + pos);
    ib_Vertices[idx + 14] = scale * (g_Vertices[7] + pos);
    ib_Vertices[idx + 15] = scale * (g_Vertices[6] + pos);

    // left
    ib_Vertices[idx + 16] = scale * (g_Vertices[0] + pos);
    ib_Vertices[idx + 17] = scale * (g_Vertices[3] + pos);
    ib_Vertices[idx + 18] = scale * (g_Vertices[7] + pos);
    ib_Vertices[idx + 19] = scale * (g_Vertices[4] + pos);

    // right
    ib_Vertices[idx + 20] = scale * (g_Vertices[1] + pos);
    ib_Vertices[idx + 21] = scale * (g_Vertices[2] + pos);
    ib_Vertices[idx + 22] = scale * (g_Vertices[6] + pos);
    ib_Vertices[idx + 23] = scale * (g_Vertices[5] + pos);
})";

