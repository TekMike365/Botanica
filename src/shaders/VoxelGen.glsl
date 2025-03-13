#version 430 core
layout (local_size_x = 32, local_size_y = 1, local_size_z = 1) in;


layout (std430, binding = 0) buffer ssboCounter { uint ib_Counter[]; };
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

bool IsAirAtPos(uvec3 pos)
{
    if (pos.x < 0 || pos.x > uVoxelsSize.x ||
        pos.y < 0 || pos.y > uVoxelsSize.y ||
        pos.z < 0 || pos.z > uVoxelsSize.z)
    {
        return true;
    }

    uint idx = pos.x + uVoxelsSize.x * (pos.y + pos.z * uVoxelsSize.y);
    uint vID = ib_VoxelIDs[idx];

    if (uDrawEnvironment == 0)
    {
        return vID == 0 || vID == 2 || vID == 6;
    }

    return vID == 0;
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
    
    if (idx > uVoxelsSize.x * uVoxelsSize.y * uVoxelsSize.z) 
        return;

    uint vID = ib_VoxelIDs[idx];
    if (vID == 0)
        return;

    // 2 - water, 6 - soil
    if (uDrawEnvironment == 0 && (vID == 2 || vID == 6))
        return;

    uvec3 globalPos = GetGlobalPosition(idx);
    vec4 pos = vec4(globalPos, 0.0);
    vec4 colorID = vec4(0.0, 0.0, 0.0, (vID - 1) * 3);

    uint FACE_BOTTOM = 0;
    uint FACE_TOP = 1;
    uint FACE_LEFT = 2;
    uint FACE_RIGHT = 3;
    uint FACE_BACK = 4;
    uint FACE_FRONT = 5;
    bool includeFaces[6] = 
    {
        IsAirAtPos(uvec3(globalPos.x, globalPos.y - 1, globalPos.z)),
        IsAirAtPos(uvec3(globalPos.x, globalPos.y + 1, globalPos.z)),
        IsAirAtPos(uvec3(globalPos.x - 1, globalPos.y, globalPos.z)),
        IsAirAtPos(uvec3(globalPos.x + 1, globalPos.y, globalPos.z)),
        IsAirAtPos(uvec3(globalPos.x, globalPos.y, globalPos.z + 1)),
        IsAirAtPos(uvec3(globalPos.x, globalPos.y, globalPos.z - 1)),
    };

    uint faceCount = 0;
    for (int i = 0; i < 6; i++)
        if (includeFaces[i])
            faceCount++;

    uint faceID = atomicAdd(ib_Counter[0], faceCount);
    uint firstVertIdx = faceID * 4;

    uint vertIdx = 0;
    if (includeFaces[FACE_BOTTOM])
    {
        ib_Vertices[firstVertIdx + vertIdx + 0] = uVoxelScale * (g_Vertices[0] + pos) + colorID;
        ib_Vertices[firstVertIdx + vertIdx + 1] = uVoxelScale * (g_Vertices[1] + pos) + colorID;
        ib_Vertices[firstVertIdx + vertIdx + 2] = uVoxelScale * (g_Vertices[2] + pos) + colorID;
        ib_Vertices[firstVertIdx + vertIdx + 3] = uVoxelScale * (g_Vertices[3] + pos) + colorID;
        vertIdx += 4;
    }
    
    if (includeFaces[FACE_TOP])
    {
        ib_Vertices[firstVertIdx + vertIdx + 0] = uVoxelScale * (g_Vertices[4] + pos) + colorID;
        ib_Vertices[firstVertIdx + vertIdx + 1] = uVoxelScale * (g_Vertices[5] + pos) + colorID;
        ib_Vertices[firstVertIdx + vertIdx + 2] = uVoxelScale * (g_Vertices[6] + pos) + colorID;
        ib_Vertices[firstVertIdx + vertIdx + 3] = uVoxelScale * (g_Vertices[7] + pos) + colorID;
        vertIdx += 4;
    }
    
    if (includeFaces[FACE_FRONT])
    {
        ib_Vertices[firstVertIdx + vertIdx + 0] = uVoxelScale * (g_Vertices[0] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
        ib_Vertices[firstVertIdx + vertIdx + 1] = uVoxelScale * (g_Vertices[1] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
        ib_Vertices[firstVertIdx + vertIdx + 2] = uVoxelScale * (g_Vertices[5] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
        ib_Vertices[firstVertIdx + vertIdx + 3] = uVoxelScale * (g_Vertices[4] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
        vertIdx += 4;
    }
    
    if (includeFaces[FACE_BACK])
    {   
        ib_Vertices[firstVertIdx + vertIdx + 0] = uVoxelScale * (g_Vertices[2] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
        ib_Vertices[firstVertIdx + vertIdx + 1] = uVoxelScale * (g_Vertices[3] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
        ib_Vertices[firstVertIdx + vertIdx + 2] = uVoxelScale * (g_Vertices[7] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
        ib_Vertices[firstVertIdx + vertIdx + 3] = uVoxelScale * (g_Vertices[6] + pos) + colorID + vec4(0.0, 0.0, 0.0, 1.0);
        vertIdx += 4;
    }
    
    if (includeFaces[FACE_LEFT])
    {   
        ib_Vertices[firstVertIdx + vertIdx + 0] = uVoxelScale * (g_Vertices[0] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
        ib_Vertices[firstVertIdx + vertIdx + 1] = uVoxelScale * (g_Vertices[3] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
        ib_Vertices[firstVertIdx + vertIdx + 2] = uVoxelScale * (g_Vertices[7] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
        ib_Vertices[firstVertIdx + vertIdx + 3] = uVoxelScale * (g_Vertices[4] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
        vertIdx += 4;
    }

    if (includeFaces[FACE_RIGHT])
    {   
        ib_Vertices[firstVertIdx + vertIdx + 0] = uVoxelScale * (g_Vertices[1] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
        ib_Vertices[firstVertIdx + vertIdx + 1] = uVoxelScale * (g_Vertices[2] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
        ib_Vertices[firstVertIdx + vertIdx + 2] = uVoxelScale * (g_Vertices[6] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
        ib_Vertices[firstVertIdx + vertIdx + 3] = uVoxelScale * (g_Vertices[5] + pos) + colorID + vec4(0.0, 0.0, 0.0, 2.0);
        vertIdx += 4;
    }
}