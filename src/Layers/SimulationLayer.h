#pragma once

#include "Layer.h"
#include "Components/World.h"
#include "Components/Plant.h"

#include "Event/KeyboardEvent.h"

class SimulationLayer : public Layer
{
public:
    SimulationLayer(std::shared_ptr<World> world);

    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep dt) override;
    virtual void OnEvent(Event &e) override;

private:
    bool OnKeyReleased(KeyReleasedEvent &e);

    void PlantAPlant(glm::uvec2 xzPos);
    void GenerateTerrain();

private:
    std::shared_ptr<World> m_World;
    std::vector<Plant> m_Plants;
    float m_Timer = 0;
    bool m_Paused = true;
    bool m_Step = false;
    int m_TickCounter = 0;
    int m_NextID = 0;
    uint32_t m_WorldSeed = 0;

    const int SPAWN_PLANT_COUNT = 20;
    const float HEIGHT_SCALAR = 16.0;
    const int WATER_LEVEL = 6;
    const int TPS = 1;
};