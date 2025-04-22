#pragma once

#include "Saba/Core/Layer.h"

namespace Saba {

class LayerStack
{
    friend class Application;
public:
    LayerStack() = default;
    SB_CORE ~LayerStack();

    SB_CORE Layer* PushLayer(Layer* layer);
    SB_CORE Layer* PopLayer(Layer* layer);
    SB_CORE Layer* PushOverlay(Layer* overlay);
    SB_CORE Layer* PopOverlay(Layer* overlay);

    std::vector<Layer*>::iterator begin() noexcept { return m_Layers.begin(); }
    std::vector<Layer*>::iterator end() noexcept { return m_Layers.end(); }
    std::vector<Layer*>::const_iterator begin() const noexcept { return m_Layers.begin(); }
    std::vector<Layer*>::const_iterator end() const noexcept { return m_Layers.end(); }

    std::vector<Layer*>::reverse_iterator rbegin() noexcept { return m_Layers.rbegin(); }
    std::vector<Layer*>::reverse_iterator rend() noexcept { return m_Layers.rend(); }
    std::vector<Layer*>::const_reverse_iterator rbegin() const noexcept { return m_Layers.rbegin(); }
    std::vector<Layer*>::const_reverse_iterator rend() const noexcept { return m_Layers.rend(); }
private:
    void Init();
    void Shutdown();
private:
    std::vector<Layer*> m_Layers;
    uint32_t m_LayerInsertIndex = 0u;
    bool m_IsInitialized = false;
};

} //namespace Saba
