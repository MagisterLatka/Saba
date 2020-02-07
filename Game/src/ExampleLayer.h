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

	Saba::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPos = { 0.0f, 0.0f, 0.0f };
	float m_CameraRotation = 0.0f;
	float m_CameraVelocity = 3.0f; //units per second
	float m_CameraRotationSpeed = 50.0f; // degrees per second

	Saba::ParticleSystem m_ParticleSystem;
	Saba::ParticleProps m_Particle;
};
