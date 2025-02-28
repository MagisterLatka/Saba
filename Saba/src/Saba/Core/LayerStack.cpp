#include <pch.h>
#include "LayerStack.h"

namespace Saba {

LayerStack::~LayerStack()
{
    for (auto* layer : m_Layers)
    {
        layer->OnDetach();
        delete layer;
    }
}

void LayerStack::Init()
{
    m_IsInitialized = true;
    for (auto* layer : m_Layers | std::views::reverse)
        layer->OnAttach();
}
void LayerStack::Shutdown()
{
    for (auto* layer : m_Layers) {
        layer->OnDetach();
        delete layer;
    }
    m_Layers.clear();
}

Layer* LayerStack::PushLayer(Layer* layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    ++m_LayerInsertIndex;
    if (m_IsInitialized)
        layer->OnAttach();
    return layer;
}
Layer* LayerStack::PushOverlay(Layer* overlay)
{
    m_Layers.emplace_back(overlay);
    if (m_IsInitialized)
        overlay->OnAttach();
    return overlay;
}
Layer* LayerStack::PopLayer(Layer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
    if (it != m_Layers.begin() + m_LayerInsertIndex) {
        if (m_IsInitialized)
            layer->OnDetach();
        m_Layers.erase(it);
        --m_LayerInsertIndex;
    }
    return layer;
}
Layer* LayerStack::PopOverlay(Layer* overlay)
{
    auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
    if (it != m_Layers.end()) {
        if (m_IsInitialized)
            overlay->OnDetach();
        m_Layers.erase(it);
    }
    return overlay;
}

} //namespace Saba
