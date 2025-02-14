#pragma once

class ExampleLayer : public Saba::Layer
{
    friend Saba::Application* Saba::CreateApplication();
public:
    ExampleLayer();
    ~ExampleLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Saba::Timestep ts) override;
    virtual void OnUIRender() override;
    virtual void OnEvent(Saba::Event& e) override;
private:
    double m_Time = 0.0;
};
