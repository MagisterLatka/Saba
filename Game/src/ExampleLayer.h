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
	Saba::Ref<Saba::VertexArray> m_VAO;
	Saba::Ref<Saba::Shader> m_Shader;

	Saba::OrthographicCameraControler m_CameraControler;

	Saba::ParticleSystem m_ParticleSystem;
	Saba::ParticleProps m_Particle;
};
