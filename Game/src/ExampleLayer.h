#pragma once

class ExampleLayer : public Saba::Layer
{
public:
	ExampleLayer();
	~ExampleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(Saba::Event& event) override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
private:
	std::shared_ptr<Saba::VertexArray> m_VAO;
	std::shared_ptr<Saba::Shader> m_Shader;
	Saba::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPos = { 0.0f, 0.0f, 0.0f };
	float m_CameraRotation = 0.0f;
};
