#include "pch.h"
#include "LayerStack.h"

LayerStack::LayerStack()
{
    m_LayerInsert = m_Layers.begin();
}

LayerStack::~LayerStack()
{
    for (Layer *layer : m_Layers)
        delete layer;
}

void LayerStack::PushLayer(Layer *layer)
{
    ResizeLayers();

    m_Layers.insert(m_LayerInsert, layer);
    m_LayerInsert++;
    layer->OnAttach();
}

void LayerStack::PushOverlay(Layer *overlay)
{
    ResizeLayers();

    m_Layers.push_back(overlay);
    overlay->OnAttach();
}

void LayerStack::PopLayer(Layer *layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
        m_Layers.erase(it);
        m_LayerInsert--;
        layer->OnDetach();
        return;
    }
}

void LayerStack::PopOverlay(Layer *overlay)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
    if (it != m_Layers.end())
    {
        m_Layers.erase(it);
        overlay->OnDetach();
        return;
    }
}

void LayerStack::ResizeLayers()
{
    if (m_Layers.capacity() > m_Layers.size())
        return;

    int idx = m_LayerInsert - m_Layers.begin();
    int allocate = 2;
    m_Layers.reserve(m_Layers.size() + allocate);

    m_LayerInsert = m_Layers.begin() + idx;
}