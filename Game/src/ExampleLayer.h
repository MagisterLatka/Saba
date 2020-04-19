#pragma once

class ExampleLayer : public Saba::Layer
{
public:
	ExampleLayer();
	~ExampleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(Saba::Event& event) override;
	virtual void OnUpdate(Saba::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	bool OnKeyPress(Saba::KeyPressedEvent& e);
private:
	Saba::PerspectiveCameraControler m_CameraControler;
	Saba::Scene3D m_Scene;
};
