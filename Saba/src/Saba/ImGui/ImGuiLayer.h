#pragma once

#include "Saba/Core/Layer.h"
#include <imgui.h>

namespace Saba {

class ImGuiLayer : public Layer {
public:
    SB_CORE ImGuiLayer() : Layer("ImGuiLayer") {}
    SB_CORE ~ImGuiLayer() = default;

    SB_CORE ImGuiContext* GetContext() const noexcept { return m_Context; }

    SB_CORE void Begin();
    SB_CORE void End();

    SB_CORE void OnAttach() override;
    SB_CORE void OnDetach() override;
    SB_CORE void OnEvent(Event& e) override;

    SB_CORE void BlockEvents(bool block) noexcept { m_BlockEvents = block; }
private:
    SB_CORE void Init();
private:
    ImGuiContext* m_Context = nullptr;
    bool m_BlockEvents = true;
};

}
