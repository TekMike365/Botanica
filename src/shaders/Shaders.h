#pragma once

const char *VoxelFrag_glsl = R"(
#version 430 core

in vec4 vColor;

out vec4 fColor;

void main()
{
    fColor = vColor;
})";

const char *WorldBoundsVert_glsl = R"(
#version 430 core

layout (location = 0) in vec3 aPos;

uniform mat4 uVP;

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
})";

const char *VoxelVert_glsl = R"(
#version 430 core

layout (location = 0) in vec4 aPos;

uniform mat4 uVP;

out vec4 vColor;

vec4 g_Colors[] = {
    // gray
    vec4(0.53, 0.51, 0.53, 1.0),
    vec4(0.39, 0.39, 0.40, 1.0),
    vec4(0.27, 0.27, 0.31, 1.0),
    // blue
    vec4(0.64, 0.86, 0.78, 1.0),
    vec4(0.41, 0.76, 0.83, 1.0),
    vec4(0.29, 0.50, 0.79, 1.0),
    // red
    vec4(0.93, 0.88, 0.62, 1.0),
    vec4(0.83, 0.63, 0.41, 1.0),
    vec4(0.71, 0.32, 0.32, 1.0),
    // brown
    vec4(0.90, 0.81, 0.71, 1.0),
    vec4(0.65, 0.48, 0.36, 1.0),
    vec4(0.50, 0.29, 0.23, 1.0),
    // green
    vec4(0.76, 0.83, 0.41, 1.0),
    vec4(0.54, 0.69, 0.38, 1.0),
    vec4(0.34, 0.48, 0.47, 1.0),
    // dark green
    vec4(0.56, 0.63, 0.21, 1.0),
    vec4(0.34, 0.49, 0.18, 1.0),
    vec4(0.14, 0.28, 0.27, 1.0),
};

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    vColor = g_Colors[uint(aPos.w) % (6 * 3)];
})";

const char *VoxelGen_glsl = R"(
#version 430 core
layout (local_size_x = 32, local_size_y = 1, local_size_z = 1) in;

layout (std430, binding = 1) buffer ssboVoxels { uint ib_VoxelIDs[]; };
layout (std430, binding = 2) buffer ssboVertices { vec4 ib_Vertices[]; };

uniform uvec3 uVoxelsSize;
uniform float uVoxelScale;
uniform int uDrawEnvironment;

uint GetGlobalInvocationIndex()
{
    return (gl_WorkGroupID.x + gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y)
           * gl_WorkGroupSize.x * gl_WorkGroupSize.y * gl_WorkGroupSize.z
           + gl_LocalInvocationIndex;
}

uvec3 GetGlobalPosition(uint idx)
{
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
    uint idx = GetGlobalInvocationIndex();
    uint vertIdx = idx * 24;

    if (idx > uVoxelsSize.x * uVoxelsSize.y * uVoxelsSize.z) 
        return;

    uint vID = ib_VoxelIDs[idx];
    if (vID == 0)
    {
        for (int i = 0; i < 24; i++)
            ib_Vertices[vertIdx + i] = vec4(0.0, 0.0, 0.0, 0.0);
        return;
    }

    // 2 - water, 6 - soil
    if (uDrawEnvironment == 0 && (vID == 2 || vID == 6))
    {
        for (int i = 0; i < 24; i++)
            ib_Vertices[vertIdx + i] = vec4(0.0, 0.0, 0.0, 0.0);
        return;
    }

    vec4 pos = vec4(GetGlobalPosition(idx), 0.0);
    vec4 colorID = vec4(0.0, 0.0, 0.0, (vID - 1) * 3);

    // bottom
    ib_Vertices[vertIdx + 0]  = uVoxelScale * (g_Vertices[0] + pos) + colorID;
    ib_Vertices[vertIdx + 1]  = uVoxelScale * (g_Vertices[1] + pos) + colorID;
    ib_Vertices[vertIdx + 2]  = uVoxelScale * (g_Vertices[2] + pos) + colorID;
    ib_Vertices[vertIdx + 3]  = uVoxelScale * (g_Vertices[3] + pos) + colorID;

    // top
    ib_Vertices[vertIdx + 4]  = uVoxelScale * (g_Vertices[4] + pos) + colorID;
    ib_Vertices[vertIdx + 5]  = uVoxelScale * (g_Vertices[5] + pos) + colorID;
    ib_Vertices[vertIdx + 6]  = uVoxelScale * (g_Vertices[6] + pos) + colorID;
    ib_Vertices[vertIdx + 7]  = uVoxelScale * (g_Vertices[7] + pos) + colorID;

    // front
    ib_Vertices[vertIdx + 8]  = uVoxelScale * (g_Vertices[0] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
    ib_Vertices[vertIdx + 9]  = uVoxelScale * (g_Vertices[1] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
    ib_Vertices[vertIdx + 10] = uVoxelScale * (g_Vertices[5] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
    ib_Vertices[vertIdx + 11] = uVoxelScale * (g_Vertices[4] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);

    // back
    ib_Vertices[vertIdx + 12] = uVoxelScale * (g_Vertices[2] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
    ib_Vertices[vertIdx + 13] = uVoxelScale * (g_Vertices[3] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
    ib_Vertices[vertIdx + 14] = uVoxelScale * (g_Vertices[7] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
    ib_Vertices[vertIdx + 15] = uVoxelScale * (g_Vertices[6] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);

    // left
    ib_Vertices[vertIdx + 16] = uVoxelScale * (g_Vertices[0] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
    ib_Vertices[vertIdx + 17] = uVoxelScale * (g_Vertices[3] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
    ib_Vertices[vertIdx + 18] = uVoxelScale * (g_Vertices[7] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
    ib_Vertices[vertIdx + 19] = uVoxelScale * (g_Vertices[4] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);

    // right
    ib_Vertices[vertIdx + 20] = uVoxelScale * (g_Vertices[1] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
    ib_Vertices[vertIdx + 21] = uVoxelScale * (g_Vertices[2] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
    ib_Vertices[vertIdx + 22] = uVoxelScale * (g_Vertices[6] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
    ib_Vertices[vertIdx + 23] = uVoxelScale * (g_Vertices[5] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
})";

const char *WorldBoundsFrag_glsl = R"(
#version 430 core

out vec4 fColor;

void main()
{
    fColor = vec4(0.2, 1.0, 0.4, 1.0);
})";

