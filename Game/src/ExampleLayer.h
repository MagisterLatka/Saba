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
	uint m_VAO, m_VBO, m_IBO;
	uint m_Shader;
};
