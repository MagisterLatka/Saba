#include <pch.h>
#include <Saba.h>
#include "ExampleLayer.h"

ExampleLayer::ExampleLayer() {

}
ExampleLayer::~ExampleLayer() {

}

void ExampleLayer::OnAttach() {

}
void ExampleLayer::OnDetach() {

}
void ExampleLayer::OnUpdate([[maybe_unused]] Saba::Timestep ts) {
    Saba::Application::Get().GetWindow()->Clear();
    auto pos = Saba::Application::Get().GetWindow()->GetMouse().GetPos();
    m_Title = std::to_string(pos.x) + ", " + std::to_string(pos.y);
}
void ExampleLayer::OnUIRender() {

}
void ExampleLayer::OnEvent(Saba::Event& e) {
    Saba::Dispatcher dispatcher(e);
    dispatcher.Dispatch<Saba::MouseButtonPressedEvent>(SB_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
}
bool ExampleLayer::OnMouseButtonPressed(Saba::MouseButtonPressedEvent& e) {
    if (e.GetButton() == Saba::MouseCode::ButtonLeft) {
        SB_TRACE("Set title to `{0}`", m_Title);
        Saba::Application::Get().GetWindow()->SetTitle(m_Title);
    }
    return false;
}
