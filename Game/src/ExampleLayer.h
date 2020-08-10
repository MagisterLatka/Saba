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
	Saba::OrthographicCameraControler m_CameraControler;

	float m_QuadFrequency = 0.1f;

	Saba::ParticleSystem m_ParticleSystem;
	Saba::ParticleProps m_Particle;
	bool m_EnableParticles = false;
};
