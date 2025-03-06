#pragma once

class ExampleLayer : public Saba::Layer
{
    friend Saba::Application* Saba::CreateApplication();
public:
    ExampleLayer();
    ~ExampleLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Saba::Timestep ts) override;
    void OnUIRender() override;
private:
    Ref<Saba::RenderPass> m_RenderPass;
    Ref<Saba::Texture2D> m_Texture;

    Ref<Saba::Scene> m_Scene;
    Saba::Entity m_Quad;

    glm::vec2 m_ViewportSize = { -1.0f, -1.0f }, m_ViewportPos;
    bool m_ViewportHovered = false, m_ViewportFocused = false;
};
