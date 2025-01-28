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

    SoilResources() = default;
    SoilResources(int v)
        : Potassium(v), Phosphorus(v), Nitrogen(v) {}
    SoilResources(int potassium, int phosphorus, int nitrogen)
        : Potassium(potassium), Phosphorus(phosphorus), Nitrogen(nitrogen) {}
};

class World
{
    friend class RenderingLayer;

public:
    World() = default;
    World(glm::uvec3 size)
        : m_Size(size), m_VoxelIDs(size, 0), m_SoilResources(size, SoilResources(MAX_POTASSIUM, MAX_PHOSPHORUS, MAX_NITROGEN)) {}

    inline void ResetResources() { m_SoilResources = Vector3D<SoilResources>(m_Size, SoilResources(MAX_POTASSIUM, MAX_PHOSPHORUS, MAX_NITROGEN)); }

    inline glm::uvec3 GetSize() const { return m_Size; }

    inline void SetVoxel(glm::uvec3 pos, uint32_t voxelType) { m_VoxelIDs[pos] = voxelType; }
    inline uint32_t GetVoxel(glm::uvec3 pos) const { return m_VoxelIDs[pos]; }

public:
    static inline int MAX_POTASSIUM = 30;
    static inline int MAX_PHOSPHORUS = 30;
    static inline int MAX_NITROGEN = 30;

private:
    glm::uvec3 m_Size;
    Vector3D<uint32_t> m_VoxelIDs;
    Vector3D<SoilResources> m_SoilResources;
};