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
    void OnEvent(Saba::Event& e) override;
private:
    bool OnMouseButtonPressed(Saba::MouseButtonPressedEvent& e);
private:
    Ref<Saba::Shader> m_Shader;
    Ref<Saba::InputLayout> m_InputLayout;
};
