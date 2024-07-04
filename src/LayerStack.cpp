#include "LayerStack.h"

namespace Botanica
{

    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)
            delete layer;
    }

    void LayerStack::PushLayer(Layer *layer)
    {
        m_Layers.emplace_back(layer);
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it == m_Layers.end())
            return;

        m_Layers.erase(it);
    }

}
