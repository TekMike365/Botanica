#pragma once

#include <array>
#include <memory>

#include "World.h"

struct PlantDNA
{
    // one per grow action (Root, Leaf, Stem, Fruit)
    std::array<int, 4> GrowthChoice;
    // one per cell in 3D (26 cells around a center one)
    std::array<int, 26> RootGrowAction;
    // one per cell in 3D (26 cells around a center one)
    std::array<int, 26> LeafGrowAction;

    static inline int MAX_VALUE = 100;
};

class Plant
{
public:
    Plant(std::shared_ptr<World> world, glm::uvec3 pos);
    Plant(std::shared_ptr<World> world, glm::uvec3 pos, const PlantDNA &dna);
    ~Plant();

    void Mine();
    void Grow();

    bool IsAlive() const;
    inline uint32_t GetSize() const { return m_RootPositions.size() + m_StemPositions.size() + m_LeafPositions.size() + m_FruitPositions.size(); }

private:
    void Init();
    bool Seed(glm::uvec2 xzPos);
    void Mutate();

    void GrowRoot(glm::uvec3 origin);
    void GrowLeaf(glm::uvec3 origin);
    void GrowStem(glm::uvec3 origin);
    void GrowFruit(glm::uvec3 origin);

private:
    std::shared_ptr<World> m_World;
    glm::uvec3 m_Pos;
    PlantDNA m_DNA;

    std::vector<glm::uvec3> m_RootPositions;
    std::vector<glm::uvec3> m_LeafPositions;
    std::vector<glm::uvec3> m_StemPositions;
    std::vector<glm::uvec3> m_FruitPositions;

    SoilResources m_SoilResources;
};