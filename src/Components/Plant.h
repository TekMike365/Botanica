#pragma once

#include <array>
#include <memory>

#include "World.h"

struct PlantDNA
{
    // one per grow action (Root, Leaf, Stem, Fruit)
    static const int GROWTH_CHOICE_LEN = 4;
    std::array<int, 4> GrowthChoice;
    // one per cell in 3D (26 cells around a center one)
    static const int ROOT_GROW_ACTION_LEN = 27;
    std::array<int, 27> RootGrowAction;
    // one per cell in 3D (26 cells around a center one)
    static const int LEAF_GROW_ACTION_LEN = 27;
    std::array<int, ROOT_GROW_ACTION_LEN> LeafGrowAction;

    static const int MAX_VALUE = 100;
    static const int MAX_GROW_CHOICE_VALUE = 10;
    static const int MIN_GROW_ACTION_VALUE = 10;
};

class Plant
{
public:
    Plant(int id, std::shared_ptr<World> world, glm::uvec3 pos);
    Plant(int id, std::shared_ptr<World> world, glm::uvec3 pos, const PlantDNA &dna);

    void Mine();
    std::vector<Plant> Reproduce(int &nextID);
    void Grow();
    void Survive();
    void Die();

    bool IsAlive() const;
    glm::uvec3 GetPosition() const { return m_Pos; }
    inline uint32_t GetSize() const { return m_RootPositions.size() + m_StemPositions.size() + m_LeafPositions.size() + m_FruitPositions.size(); }
    inline int GetID() const { return m_ID; }

public:
    static constexpr SoilResources SOIL_STORAGE_MPLR = {
        20,
        20,
        20,
    };
    static const int WATER_STORAGE_MPLR = 20;
    static const int LIGHT_STORAGE_MPLR = 20;

    static constexpr float WATER_MINE_MPLR = 0.8f;
    static constexpr float LIGHT_MINE_MPLR = 0.9f;
    static constexpr float SOIL_MINE_MPLR = 0.9f;
    static constexpr float SOIL_WATER_MPLR = 0.2;

    static constexpr float POTASSIUM_BONUS_CONSTANT = 1.2f;
    static constexpr float PHOSPHORUS_BONUS_CONSTANT = 1.3f;
    static constexpr float NITROGEN_BONUS_CONSTANT = 1.3f;

    static const int PLANT_PART_WATER_COST = 5;
    static const int PLANT_PART_LIGHT_COST = 5;

    static constexpr float WATER_SURVIVE_COST_MPLR = 0.8f;
    static constexpr float LIGHT_SURVIVE_COST_MPLR = 0.9f;
    static constexpr float POTASSIUM_SURVIVE_COST_MPLR = 0.9f;
    static constexpr float PHOSPHORUS_SURVIVE_COST_MPLR = 0.9f;
    static constexpr float NITROGEN_SURVIVE_COST_MPLR = 0.9f;

    static constexpr float START_RESOURCES_MPLR = 0.75f;

    static const int ROOT_POTASSIUM_COST = 10;
    static const int ROOT_NITROGEN_COST = 7;

    static const int LEAF_PHOSPHORUS_COST = 7;

    static const int STEM_PHOSPHORUS_COST = 7;
    static const int STEM_POTASSIUM_COST = 7;
    static const int STEM_NITROGEN_COST = 7;

    static const int FRUIT_PHOSPHORUS_COST = 15;
    static const int FRUIT_POTASSIUM_COST = 15;
    static const int FRUIT_NITROGEN_COST = 15;

private:
    void Init();
    void Mutate();

    void MineSoil(glm::uvec3 pos);
    void MineAir(glm::uvec3 pos);

    Plant Seed(glm::uvec2 xzPos, int id);

    void GrowRoot();
    void GrowLeaf();
    void GrowStem();
    void GrowFruit();

    inline int GetRemainingWaterCapacity() const { return WATER_STORAGE_MPLR * m_StemPositions.size() - m_Water; }
    inline int GetRemainingLightCapacity() const { return LIGHT_STORAGE_MPLR * m_StemPositions.size() - m_Light; }
    inline SoilResources GetRemainingSoilResourcesCapacity() const { return {
        SOIL_STORAGE_MPLR.Potassium * (int)m_StemPositions.size() - m_SoilResources.Potassium,
        SOIL_STORAGE_MPLR.Phosphorus * (int)m_StemPositions.size() - m_SoilResources.Phosphorus,
        SOIL_STORAGE_MPLR.Nitrogen * (int)m_StemPositions.size() - m_SoilResources.Nitrogen}; }

    inline int GetWaterBonus() const { return m_SoilResources.Potassium / GetSize() * POTASSIUM_BONUS_CONSTANT; }
    inline int GetLightBonus() const { return m_SoilResources.Phosphorus / GetSize() * PHOSPHORUS_BONUS_CONSTANT; }
    inline int GetSoilBonus() const { return m_SoilResources.Nitrogen / GetSize() * NITROGEN_BONUS_CONSTANT; }

private:
    std::shared_ptr<World> m_World;
    glm::uvec3 m_Pos;
    PlantDNA m_DNA;
    int m_ID;

    std::vector<glm::uvec3> m_RootPositions;
    std::vector<glm::uvec3> m_LeafPositions;
    std::vector<glm::uvec3> m_StemPositions;
    std::vector<glm::uvec3> m_FruitPositions;

    SoilResources m_SoilResources;
    int m_Water = 0;
    int m_Light = 0;
};