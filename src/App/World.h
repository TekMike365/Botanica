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

        void SetVoxel(glm::uvec3 pos, uint8_t type);
        uint8_t GetVoxel(glm::uvec3 pos) const;

        inline glm::uvec3 GetSize() const { return m_Size; }
        inline const std::vector<uint8_t> &GetVoxels() const { return m_Voxels; }

    private:
        glm::uvec3 m_Size;
        std::vector<uint8_t> m_Voxels;
    };
}