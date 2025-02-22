#pragma once

#include "Saba/Core/Layer.h"

namespace Saba {

class LayerStack
{
    friend class Application;
public:
    SB_CORE LayerStack() = default;
    SB_CORE ~LayerStack();

    SB_CORE Layer* PushLayer(Layer* layer);
    SB_CORE Layer* PopLayer(Layer* layer);
    SB_CORE Layer* PushOverlay(Layer* overlay);
    SB_CORE Layer* PopOverlay(Layer* overlay);

    SB_CORE std::vector<Layer*>::iterator begin() noexcept { return m_Layers.begin(); }
    SB_CORE std::vector<Layer*>::iterator end() noexcept { return m_Layers.end(); }
    SB_CORE std::vector<Layer*>::const_iterator begin() const noexcept { return m_Layers.begin(); }
    SB_CORE std::vector<Layer*>::const_iterator end() const noexcept { return m_Layers.end(); }

    SB_CORE std::vector<Layer*>::reverse_iterator rbegin() noexcept { return m_Layers.rbegin(); }
    SB_CORE std::vector<Layer*>::reverse_iterator rend() noexcept { return m_Layers.rend(); }
    SB_CORE std::vector<Layer*>::const_reverse_iterator rbegin() const noexcept { return m_Layers.rbegin(); }
    SB_CORE std::vector<Layer*>::const_reverse_iterator rend() const noexcept { return m_Layers.rend(); }
private:
    SB_CORE void Init();
    SB_CORE void Shutdown();
private:
    std::vector<Layer*> m_Layers;
    uint32_t m_LayerInsertIndex = 0u;
    bool m_IsInitialized = false;
};

} //namespace Saba
