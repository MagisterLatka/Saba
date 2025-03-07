#pragma once

class ExampleLayer : public Saba::Layer
{
    friend Saba::Application* Saba::CreateApplication();
public:
    ExampleLayer();
    ~ExampleLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Saba::Event& e) override;
    void OnUpdate(Saba::Timestep ts) override;
    void OnUIRender() override;
private:
    Ref<Saba::RenderPass> m_RenderPass;
    Ref<Saba::Texture2D> m_Texture;

    Ref<Saba::Scene> m_Scene;
    Saba::Entity m_Quad, m_Camera;

    glm::uvec2 m_ViewportSize = { 0, 0 };
    glm::ivec2 m_ViewportPos;
    bool m_ViewportHovered = false, m_ViewportFocused = false;
};
