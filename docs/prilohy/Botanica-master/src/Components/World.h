#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "Vector3D.h"

enum VoxelType
{
    VoxelTypeAir = 0,
    VoxelTypeRoot = 1,
    VoxelTypeWater = 2,
    VoxelTypeFruit = 3,
    VoxelTypeStem = 4,
    VoxelTypeLeaf = 5,
    VoxelTypeSoil = 6
};

struct SoilResources
{
    int Potassium = 0;
    int Phosphorus = 0;
    int Nitrogen = 0;
};

class World
{
    friend class RenderingLayer;

public:
    World() = default;
    World(glm::uvec3 size)
        : m_Size(size), m_VoxelIDs(size, 0), m_SoilResources(size, SoilResources())
    {
        ResetResources();
    }

    inline void ResetResources()
    {
        for (auto &res : m_SoilResources.GetData())
            res = {
                MAX_POTASSIUM,
                MAX_PHOSPHORUS,
                MAX_NITROGEN};
    }

    inline glm::uvec3 GetSize() const { return m_Size; }

    inline void SetVoxel(glm::uvec3 pos, uint32_t voxelType) { m_VoxelIDs[pos] = voxelType; }
    inline uint32_t GetVoxel(glm::uvec3 pos) const { return m_VoxelIDs[pos]; }

    SoilResources MineSoilResources(glm::uvec3 pos, SoilResources resources)
    {
        SoilResources mine = {
            resources.Potassium <= m_SoilResources[pos].Potassium ? resources.Potassium : m_SoilResources[pos].Potassium,
            resources.Phosphorus <= m_SoilResources[pos].Phosphorus ? resources.Phosphorus : m_SoilResources[pos].Phosphorus,
            resources.Nitrogen <= m_SoilResources[pos].Nitrogen ? resources.Nitrogen : m_SoilResources[pos].Nitrogen,
        };

        m_SoilResources[pos].Potassium -= mine.Potassium;
        m_SoilResources[pos].Phosphorus -= mine.Phosphorus;
        m_SoilResources[pos].Nitrogen -= mine.Nitrogen;

        return mine;
    }

public:
    const int MAX_POTASSIUM = 30;
    const int MAX_PHOSPHORUS = 30;
    const int MAX_NITROGEN = 30;

private:
    glm::uvec3 m_Size;
    Vector3D<uint32_t> m_VoxelIDs;
    Vector3D<SoilResources> m_SoilResources;
};