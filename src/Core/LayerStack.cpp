#include "btpch.h"
#include "LayerStack.h"

namespace Botanica
{
    LayerStack::LayerStack()
    {
        m_LayerInsert = m_Layers.begin();
    }

    Botanica::LayerStack::~LayerStack()
    {
        for (Layer *layer : m_Layers)
            delete layer;
    }

    void LayerStack::PushLayer(Layer *layer)
    {
        m_Layers.insert(m_LayerInsert, layer);
        m_LayerInsert++;
        layer->OnAttach();
        BT_CORE_INFO("Pushed layer on the stack: {}", layer->GetName());
    }

    void LayerStack::PushOverlay(Layer *overlay)
    {
        m_Layers.push_back(overlay);
        overlay->OnAttach();
        BT_CORE_INFO("Pushed overlay on the stack: {}", overlay->GetName());
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
            m_LayerInsert--;
            layer->OnDetach();
            BT_CORE_INFO("Popped layer of the stack: {}", layer->GetName());
            return;
        }
        BT_CORE_INFO("Failed to pop layer of the stack: {}", layer->GetName());
    }

    void LayerStack::PopOverlay(Layer *overlay)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
            overlay->OnDetach();
            BT_CORE_INFO("Popped overlay of the stack: {}", overlay->GetName());
            return;
        }
        BT_CORE_INFO("Failed to pop overlay of the stack: {}", overlay->GetName());
    }
}
