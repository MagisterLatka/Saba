#pragma once

#include "Saba/Core/Timestep.h"
#include "Saba/Events/Event.h"

namespace Saba {

class Layer
{
public:
    Layer(std::string name = "Layer") noexcept
        : m_Name(std::move(name)) {}
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate([[maybe_unused]] Timestep ts) {}
    virtual void OnUIRender() {}
    virtual void OnEvent([[maybe_unused]] Event& e) {}
protected:
    std::string m_Name;
};

} //namespace Saba
