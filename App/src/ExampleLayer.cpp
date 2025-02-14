#include <pch.h>
#include "Saba.h"
#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
{

}
ExampleLayer::~ExampleLayer()
{

}

void ExampleLayer::OnAttach()
{

}
void ExampleLayer::OnDetach()
{

}
void ExampleLayer::OnUpdate(Saba::Timestep ts)
{
    m_Time += ts;
    if (m_Time > 1.0)
    {
        SB_TRACE("1 sec elapsed");
        m_Time -= 1.0;
    }
}
void ExampleLayer::OnUIRender()
{

}
void ExampleLayer::OnEvent([[maybe_unused]] Saba::Event& e)
{

}
