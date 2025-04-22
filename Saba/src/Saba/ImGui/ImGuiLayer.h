#pragma once

#include "Saba/Core/Layer.h"
#include <imgui.h>

namespace Saba {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer() : Layer("ImGuiLayer") {}
    ~ImGuiLayer() = default;

    ImGuiContext* GetContext() const noexcept { return m_Context; }

    SB_CORE void Begin();
    SB_CORE void End();

    SB_CORE void OnAttach() override;
    SB_CORE void OnDetach() override;
    SB_CORE void OnEvent(Event& e) override;

    void BlockEvents(bool block) noexcept { m_BlockEvents = block; }
private:
    void Init();
private:
    ImGuiContext* m_Context = nullptr;
    bool m_BlockEvents = true;
};

} //namespace Saba
