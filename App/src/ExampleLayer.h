#pragma once

class ExampleLayer : public Saba::Layer
{
    friend Saba::Application* Saba::CreateApplication();
public:
    ExampleLayer() = default;
    ~ExampleLayer() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Saba::Event& e) override;
    void OnUpdate(Saba::Timestep ts) override;
    void OnUIRender() override;
private:
    bool OnKeyPressed(Saba::KeyPressedEvent& e);
    bool OnMouseButtonPressed(Saba::MouseButtonPressedEvent& e);
private:
    Ref<Saba::RenderPass> m_RenderPass;

    Ref<Saba::Scene> m_Scene;

    glm::uvec2 m_ViewportSize = { 0, 0 };
    glm::ivec2 m_ViewportPos = { 0, 0 };
};
