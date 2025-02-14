#pragma once

#include "Saba/Core.h"
#include "Saba/Timestep.h"
#include "Saba/Events/Event.h"

namespace Saba {

class Layer
{
public:
    SB_CORE Layer(const std::string& name = "Layer") noexcept
        : m_Name(name) {}
    SB_CORE virtual ~Layer() = default;

    SB_CORE virtual void OnAttach() {}
    SB_CORE virtual void OnDetach() {}
    SB_CORE virtual void OnUpdate([[maybe_unused]] Timestep ts) {}
    SB_CORE virtual void OnUIRender() {}
    SB_CORE virtual void OnEvent([[maybe_unused]] Event& e) {}
protected:
    std::string m_Name;
};

} //namespace Saba
