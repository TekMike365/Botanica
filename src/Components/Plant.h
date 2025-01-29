#pragma once

#include <array>
#include <memory>

#include "World.h"

struct PlantDNA
{
    // one per grow action (Root, Leaf, Stem, Fruit)
    std::array<int, 4> GrowthChoice;
    // one per cell in 3D (26 cells around a center one)
    std::array<int, 27> RootGrowAction;
    // one per cell in 3D (26 cells around a center one)
    std::array<int, 27> LeafGrowAction;

    static inline int MAX_VALUE = 100;
};

class Plant
{
public:
    Plant(std::shared_ptr<World> world, glm::uvec3 pos);
    Plant(std::shared_ptr<World> world, glm::uvec3 pos, const PlantDNA &dna);

    void Mine();
    std::vector<Plant> Reproduce();
    void Grow();
    void Survive();
    void Die();

    bool IsAlive() const;
    glm::uvec3 GetPosition() const { return m_Pos; }
    inline uint32_t GetSize() const { return m_RootPositions.size() + m_StemPositions.size() + m_LeafPositions.size() + m_FruitPositions.size(); }

private:
    void Init();
    void Mutate();

    void MineSoil(glm::uvec3 pos);
    void MineAir(glm::uvec3 pos);

    Plant Seed(glm::uvec2 xzPos);

    void GrowRoot();
    void GrowLeaf();
    void GrowStem();
    void GrowFruit();

    inline int GetRemainingWaterCapacity() const { return WATER_STORAGE_MPLR * m_StemPositions.size() - m_Water; }
    inline int GetRemainingLightCapacity() const { return LIGHT_STORAGE_MPLR * m_StemPositions.size() - m_Light; }
    inline SoilResources GetRemainingSoilResourcesCapacity() const { return {
        .Potassium = SOIL_STORAGE_MPLR.Potassium * (int)m_StemPositions.size() - m_SoilResources.Potassium,
        .Phosphorus = SOIL_STORAGE_MPLR.Phosphorus * (int)m_StemPositions.size() - m_SoilResources.Phosphorus,
        .Nitrogen = SOIL_STORAGE_MPLR.Nitrogen * (int)m_StemPositions.size() - m_SoilResources.Nitrogen}; }

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

    static constexpr SoilResources SOIL_STORAGE_MPLR = {
        .Potassium = 20,
        .Phosphorus = 20,
        .Nitrogen = 20,
    };
    const static int WATER_STORAGE_MPLR = 20;
    const static int LIGHT_STORAGE_MPLR = 20;

    static constexpr float WATER_MINE_MPLR = 1.7f;
    static constexpr float LIGHT_MINE_MPLR = 1.7f;
    static constexpr float SOIL_MINE_MPLR = 1.7f;

    static constexpr float POTASSIUM_BONUS_CONSTANT = 2.0f;
    static constexpr float PHOSPHORUS_BONUS_CONSTANT = 2.0f;
    static constexpr float NITROGEN_BONUS_CONSTANT = 2.0f;

    const static int PLANT_PART_WATER_COST = 2;
    const static int PLANT_PART_LIGHT_COST = 2;

    static constexpr float WATER_SURVIVE_COST_MPLR = 1.0f;
    static constexpr float LIGHT_SURVIVE_COST_MPLR = 1.0f;
    static constexpr float POTASSIUM_SURVIVE_COST_MPLR = 0.5f;
    static constexpr float PHOSPHORUS_SURVIVE_COST_MPLR = 0.5f;
    static constexpr float NITROGEN_SURVIVE_COST_MPLR = 0.5f;

    static constexpr float START_RESOURCES_MPLR = 0.75f;
};