#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Core/Layer.h"

namespace App
{
    using namespace Botanica;

    class World : public Layer
    {
    public:
        World();
        World(glm::uvec3 size);

        void SetVoxel(glm::uvec3 pos, uint32_t type);
        uint32_t GetVoxel(glm::uvec3 pos) const;

        inline glm::uvec3 GetSize() const { return m_Size; }
        inline const std::vector<uint32_t> &GetVoxels() const { return m_Voxels; }

    public:
        bool DataUpdated;

    private:
        glm::uvec3 m_Size;
        std::vector<uint32_t> m_Voxels;
    };
}