#pragma once

#include "btpch.h"

#include "Component.h"

namespace Botanica::ECS
{

    class System
    {
        friend class Scene;
    public:
        virtual const char* GetType() const = 0;
    protected:
        System(std::vector<Component>& components);
        virtual void OnUpdate() = 0;
    protected:
        std::vector<Component>& m_Components;
    };

}

template <>
struct std::hash<Botanica::ECS::System>
{
    size_t operator()(const Botanica::ECS::System& system)
    {
        return std::hash<std::string>()(system.GetType());
    }
};
