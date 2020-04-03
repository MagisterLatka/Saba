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
	Saba::Ref<Saba::Texture2D> m_Texture;
	Saba::Ref<Saba::Shader> m_2DShader;

	Saba::OrthographicCameraControler m_CameraControler;

	Saba::Ref<Saba::Shader> m_ParticleShader;
	Saba::ParticleSystem m_ParticleSystem;
	Saba::ParticleProps m_Particle;
	bool m_EnableParticles = false;
};
