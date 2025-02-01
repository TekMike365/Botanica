#pragma once

#include <array>
#include <memory>

#include "World.h"

struct PlantDNA
{
    // one per grow action (Root, Leaf, Stem, Fruit)
    inline static const int GROWTH_CHOICE_LEN = 4;
    std::array<int, 4> GrowthChoice;
    // one per cell in 3D (26 cells around a center one)
    inline static const int ROOT_GROW_ACTION_LEN = 27;
    std::array<int, 27> RootGrowAction;
    // one per cell in 3D (26 cells around a center one)
    inline static const int LEAF_GROW_ACTION_LEN = 27;
    std::array<int, ROOT_GROW_ACTION_LEN> LeafGrowAction;

    inline static const int MAX_VALUE = 100;
    inline static const int MIN_GROW_ACTION_VALUE = 10;
    inline static const int MIN_ROOT_GROW_CHOICE_VAL = 10;
    inline static const int MIN_STEM_GROW_CHOICE_VAL = 10;
    inline static const int MIN_LEAF_GROW_CHOICE_VAL = 10;
    inline static const int MIN_FRUIT_GROW_CHOICE_VAL = 10;
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
        .Potassium = 20,
        .Phosphorus = 20,
        .Nitrogen = 20,
    };
    inline static const int WATER_STORAGE_MPLR = 20;
    inline static const int LIGHT_STORAGE_MPLR = 20;

    inline static constexpr float WATER_MINE_MPLR = 1.2f;
    inline static constexpr float LIGHT_MINE_MPLR = 1.2f;
    inline static constexpr float SOIL_MINE_MPLR = 1.2f;
    inline static constexpr float SOIL_WATER_MPLR = 0.2;

    inline static constexpr float POTASSIUM_BONUS_CONSTANT = 1.3f;
    inline static constexpr float PHOSPHORUS_BONUS_CONSTANT = 1.3f;
    inline static constexpr float NITROGEN_BONUS_CONSTANT = 1.3f;

    inline static const int PLANT_PART_WATER_COST = 5;
    inline static const int PLANT_PART_LIGHT_COST = 5;

    inline static constexpr float WATER_SURVIVE_COST_MPLR = 0.9f;
    inline static constexpr float LIGHT_SURVIVE_COST_MPLR = 0.9f;
    inline static constexpr float POTASSIUM_SURVIVE_COST_MPLR = 0.7f;
    inline static constexpr float PHOSPHORUS_SURVIVE_COST_MPLR = 0.7f;
    inline static constexpr float NITROGEN_SURVIVE_COST_MPLR = 0.7f;

    inline static constexpr float START_RESOURCES_MPLR = 0.75f;

    inline static const int ROOT_POTASSIUM_COST = 5;
    inline static const int ROOT_NITROGEN_COST = 5;

    inline static const int LEAF_PHOSPHORUS_COST = 5;

    inline static const int STEM_PHOSPHORUS_COST = 5;
    inline static const int STEM_POTASSIUM_COST = 5;
    inline static const int STEM_NITROGEN_COST = 5;

    inline static const int FRUIT_PHOSPHORUS_COST = 7;
    inline static const int FRUIT_POTASSIUM_COST = 7;
    inline static const int FRUIT_NITROGEN_COST = 7;

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
        .Potassium = SOIL_STORAGE_MPLR.Potassium * (int)m_StemPositions.size() - m_SoilResources.Potassium,
        .Phosphorus = SOIL_STORAGE_MPLR.Phosphorus * (int)m_StemPositions.size() - m_SoilResources.Phosphorus,
        .Nitrogen = SOIL_STORAGE_MPLR.Nitrogen * (int)m_StemPositions.size() - m_SoilResources.Nitrogen}; }

    inline int GetWaterBonus() const { return m_SoilResources.Potassium / GetSize() * POTASSIUM_BONUS_CONSTANT; }
    inline int GetLightBonus() const { return m_SoilResources.Phosphorus / GetSize() * PHOSPHORUS_BONUS_CONSTANT; }
    inline int GetSoilBonus() const { return m_SoilResources.Nitrogen / GetSize() * NITROGEN_BONUS_CONSTANT; }

    inline void LogDNA() const
    {
        Log::SimTrace("[pid: {}] DNA:", m_ID);
        Log::SimTrace("[pid: {}]     GrowChoice:", m_ID);
        Log::SimTrace("[pid: {}]         RootGA: {}", m_ID, m_DNA.GrowthChoice[0]);
        Log::SimTrace("[pid: {}]         StemGA: {}", m_ID, m_DNA.GrowthChoice[1]);
        Log::SimTrace("[pid: {}]         LeafGA: {}", m_ID, m_DNA.GrowthChoice[2]);
        Log::SimTrace("[pid: {}]         FruitGA: {}", m_ID, m_DNA.GrowthChoice[3]);
        Log::SimTrace("[pid: {}]     RootGrowAction:", m_ID);
        for (int i = 0; i < m_DNA.ROOT_GROW_ACTION_LEN; i++)
            Log::SimTrace("[pid: {}]         {}: {}", m_ID, i, m_DNA.RootGrowAction[i]);
        Log::SimTrace("[pid: {}]     LeafGrowAction:", m_ID);
        for (int i = 0; i < m_DNA.LEAF_GROW_ACTION_LEN; i++)
            Log::SimTrace("[pid: {}]         {}: {}", m_ID, i, m_DNA.LeafGrowAction[i]);
    }

    inline void LogPosVector(const std::vector<glm::uvec3> &vec, const std::string &msg) const
    {
        Log::SimTrace("[pid: {}] {}:", m_ID, msg);
        for (int i = 0; i < vec.size(); i++)
            Log::SimTrace("[pid: {}]     {}: [{}, {}, {}]", m_ID, i, vec[i].x, vec[i].y, vec[i].z);
    }

    inline void LogPlantResources() const
    {
        Log::SimTrace("[pid: {}] Resources:", m_ID);
        Log::SimTrace("[pid: {}]     Water: {}", m_ID, m_Water);
        Log::SimTrace("[pid: {}]     Light: {}", m_ID, m_Light);
        Log::SimTrace("[pid: {}]     Soil Resources:", m_ID);
        Log::SimTrace("[pid: {}]         Phosphorus: {}", m_ID, m_SoilResources.Phosphorus);
        Log::SimTrace("[pid: {}]         Potassium: {}", m_ID, m_SoilResources.Potassium);
        Log::SimTrace("[pid: {}]         Nitrogen: {}", m_ID, m_SoilResources.Nitrogen);
        Log::SimTrace("[pid: {}] Survival Cost:", m_ID);
        Log::SimTrace("[pid: {}]     Water: {}", m_ID, WATER_SURVIVE_COST_MPLR * GetSize());
        Log::SimTrace("[pid: {}]     Light: {}", m_ID, LIGHT_SURVIVE_COST_MPLR * GetSize());
        Log::SimTrace("[pid: {}]     Soil Resources:", m_ID);
        Log::SimTrace("[pid: {}]         Phosphorus: {}", m_ID, NITROGEN_SURVIVE_COST_MPLR * GetSize());
        Log::SimTrace("[pid: {}]         Potassium: {}", m_ID, PHOSPHORUS_SURVIVE_COST_MPLR * GetSize());
        Log::SimTrace("[pid: {}]         Nitrogen: {}", m_ID, POTASSIUM_SURVIVE_COST_MPLR * GetSize());
    }

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