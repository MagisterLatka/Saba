#pragma once

#include <Saba/Panels/SceneHierarchyPanel.h>
#include <Saba/Panels/ContentBrowserPanel.h>

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
    bool OnKeyPressed(Saba::KeyPressedEvent& e);
    bool OnMouseButtonPressed(Saba::MouseButtonPressedEvent& e);

    void NewScene();
    void OpenScene();
    void OpenScene(const std::filesystem::path& path);
    void SaveScene();
private:
    Ref<Saba::RenderPass> m_RenderPass;
    Ref<Saba::Texture2D> m_Texture;

    Ref<Saba::Scene> m_Scene;
    Saba::Entity m_Camera;
    Scope<Saba::SceneHierarchyPanel> m_SceneHierarchyPanel;
    Scope<Saba::ContentBrowserPanel> m_ContentBrowserPanel;
    Saba::Entity m_HoveredEntity;

    glm::uvec2 m_ViewportSize = { 0, 0 };
    glm::ivec2 m_ViewportPos = { 0, 0 };
    bool m_ViewportHovered = false, m_ViewportFocused = false;
    int m_GuizmoType = -1;
};
