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
	bool OnWindowResize(Saba::WindowResizeEvent& e);
private:
	Saba::Ref<Saba::Framebuffer> m_FBO;

	Saba::ParticleSystem m_ParticleSystem;
	Saba::ParticleProps m_Particle;
	bool m_EnableParticles = false;

	Saba::Scene m_Scene;
	Saba::Entity m_Camera;

#if SB_DEBUG
	static constexpr float m_QuadFrequency = 0.25f;
#else
	static constexpr float m_QuadFrequency = 0.1f;
#endif
};
