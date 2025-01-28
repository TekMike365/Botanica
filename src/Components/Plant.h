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

    void MineSoil(glm::uvec3 pos);
    void MineAir(glm::uvec3 pos);

    void GrowRoot(glm::uvec3 origin);
    void GrowLeaf(glm::uvec3 origin);
    void GrowStem(glm::uvec3 origin);
    void GrowFruit(glm::uvec3 origin);

    inline int GetRemainingWaterCapacity() const { return WATER_STORAGE_MPLR * m_StemPositions.size() - m_Water; }
    inline int GetRemainingLightCapacity() const { return LIGHT_STORAGE_MPLR * m_StemPositions.size() - m_Light; }
    inline SoilResources GetRemainingSoilResourcesCapacity() const { return {
        .Potassium = SOIL_STORAGE_MPLR.Potassium * m_StemPositions.size() - m_SoilResources.Potassium,
        .Phosphorus = SOIL_STORAGE_MPLR.Phosphorus * m_StemPositions.size() - m_SoilResources.Phosphorus,
        .Nitrogen = SOIL_STORAGE_MPLR.Nitrogen * m_StemPositions.size() - m_SoilResources.Nitrogen}; }

    inline int GetWaterBonus() const { return m_SoilResources.Potassium / GetSize() * POTASSIUM_BONUS_CONSTANT; }
    inline int GetLightBonus() const { return m_SoilResources.Phosphorus / GetSize() * PHOSPHORUS_BONUS_CONSTANT; }
    inline int GetSoilBonus() const { return m_SoilResources.Nitrogen / GetSize() * NITROGEN_BONUS_CONSTANT; }

private:
    std::shared_ptr<World> m_World;
    glm::uvec3 m_Pos;
    PlantDNA m_DNA;

    std::vector<glm::uvec3> m_RootPositions;
    std::vector<glm::uvec3> m_LeafPositions;
    std::vector<glm::uvec3> m_StemPositions;
    std::vector<glm::uvec3> m_FruitPositions;

    SoilResources m_SoilResources;
    int m_Water;
    int m_Light;

    const SoilResources SOIL_STORAGE_MPLR = {
        .Potassium = 20,
        .Phosphorus = 20,
        .Nitrogen = 20,
    };
    const int WATER_STORAGE_MPLR = 20;
    const int LIGHT_STORAGE_MPLR = 20;

    const float WATER_MINE_MPLR = 1.0f;
    const float LIGHT_MINE_MPLR = 1.0f;
    const float SOIL_MINE_MPLR = 1.0f;

    const float POTASSIUM_BONUS_CONSTANT = 2.0f;
    const float PHOSPHORUS_BONUS_CONSTANT = 2.0f;
    const float NITROGEN_BONUS_CONSTANT = 2.0f;
};