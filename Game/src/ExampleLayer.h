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
	std::shared_ptr<Saba::VertexBuffer> m_VBO;
	std::shared_ptr<Saba::IndexBuffer> m_IBO;
	uint m_VAO;
	uint m_Shader;
};
