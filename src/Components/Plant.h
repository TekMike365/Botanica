#pragma once

#include <array>
#include <memory>

#include "World.h"

struct PlantDNA
{
    // one per grow action (Root, Leaf, Stem, Fruit)
    static inline const int GROWTH_CHOICE_LEN = 4;
    std::array<int, 4> GrowthChoice;
    // one per cell in 3D (26 cells around a center one)
    static inline const int ROOT_GROW_ACTION_LEN = 27;
    std::array<int, 27> RootGrowAction;
    // one per cell in 3D (26 cells around a center one)
    static inline const int LEAF_GROW_ACTION_LEN = 27;
    std::array<int, ROOT_GROW_ACTION_LEN> LeafGrowAction;

    static const int MAX_VALUE = 100;
    static const int MIN_GROW_ACTION_VALUE = 10;
    static const int MIN_ROOT_GROW_CHOICE_VAL = 30;
    static const int MIN_STEM_GROW_CHOICE_VAL = 10;
    static const int MIN_LEAF_GROW_CHOICE_VAL = 10;
    static const int MIN_FRUIT_GROW_CHOICE_VAL = 10;
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
        BT_DLOG_TRACE("[pid: {}] DNA:", m_ID);
        BT_DLOG_TRACE("[pid: {}]    GrowChoice:", m_ID);
        BT_DLOG_TRACE("[pid: {}]        RootGA: {}", m_ID, m_DNA.GrowthChoice[0]);
        BT_DLOG_TRACE("[pid: {}]        StemGA: {}", m_ID, m_DNA.GrowthChoice[1]);
        BT_DLOG_TRACE("[pid: {}]        LeafGA: {}", m_ID, m_DNA.GrowthChoice[2]);
        BT_DLOG_TRACE("[pid: {}]        FruitGA: {}", m_ID, m_DNA.GrowthChoice[3]);
        BT_DLOG_TRACE("[pid: {}]    RootGrowAction:", m_ID);
        for (int i = 0; i < m_DNA.ROOT_GROW_ACTION_LEN; i++)
            BT_DLOG_TRACE("[pid: {}]        {}: {}", m_ID, i, m_DNA.RootGrowAction[i]);
        BT_DLOG_TRACE("[pid: {}]    LeafGrowAction:", m_ID);
        for (int i = 0; i < m_DNA.LEAF_GROW_ACTION_LEN; i++)
            BT_DLOG_TRACE("[pid: {}]        {}: {}", m_ID, i, m_DNA.LeafGrowAction[i]);
    }

    inline void LogPosVector(const std::vector<glm::uvec3> &vec, const std::string &msg) const
    {
        BT_DLOG_TRACE("[pid: {}] {}:", m_ID, msg);
        for (int i = 0; i < vec.size(); i++)
            BT_DLOG_TRACE("[pid: {}]    {}: [{}, {}, {}]", m_ID, i, vec[i].x, vec[i].y, vec[i].z);
    }

    inline void LogPlantResources() const
    {
        BT_DLOG_TRACE("[pid: {}] Resources:", m_ID);
        BT_DLOG_TRACE("[pid: {}]    Water: {}", m_ID, m_Water);
        BT_DLOG_TRACE("[pid: {}]    Light: {}", m_ID, m_Light);
        BT_DLOG_TRACE("[pid: {}]    Soil Resources:", m_ID);
        BT_DLOG_TRACE("[pid: {}]        Phosphorus: {}", m_ID, m_SoilResources.Phosphorus);
        BT_DLOG_TRACE("[pid: {}]        Potassium: {}", m_ID, m_SoilResources.Potassium);
        BT_DLOG_TRACE("[pid: {}]        Nitrogen: {}", m_ID, m_SoilResources.Nitrogen);
        BT_DLOG_TRACE("[pid: {}] Survival Cost:", m_ID);
        BT_DLOG_TRACE("[pid: {}]    Water: {}", m_ID, WATER_SURVIVE_COST_MPLR * GetSize());
        BT_DLOG_TRACE("[pid: {}]    Light: {}", m_ID, LIGHT_SURVIVE_COST_MPLR * GetSize());
        BT_DLOG_TRACE("[pid: {}]    Soil Resources:", m_ID);
        BT_DLOG_TRACE("[pid: {}]        Phosphorus: {}", m_ID, NITROGEN_SURVIVE_COST_MPLR * GetSize());
        BT_DLOG_TRACE("[pid: {}]        Potassium: {}", m_ID, PHOSPHORUS_SURVIVE_COST_MPLR * GetSize());
        BT_DLOG_TRACE("[pid: {}]        Nitrogen: {}", m_ID, POTASSIUM_SURVIVE_COST_MPLR * GetSize());
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
    static constexpr float SOIL_WATER_MPLR = 0.8;

    static constexpr float POTASSIUM_BONUS_CONSTANT = 2.0f;
    static constexpr float PHOSPHORUS_BONUS_CONSTANT = 2.0f;
    static constexpr float NITROGEN_BONUS_CONSTANT = 2.0f;

    const static int PLANT_PART_WATER_COST = 2;
    const static int PLANT_PART_LIGHT_COST = 2;

    static constexpr float WATER_SURVIVE_COST_MPLR = 0.7f;
    static constexpr float LIGHT_SURVIVE_COST_MPLR = 0.6f;
    static constexpr float POTASSIUM_SURVIVE_COST_MPLR = 0.5f;
    static constexpr float PHOSPHORUS_SURVIVE_COST_MPLR = 0.5f;
    static constexpr float NITROGEN_SURVIVE_COST_MPLR = 0.5f;

    static constexpr float START_RESOURCES_MPLR = 0.75f;
};