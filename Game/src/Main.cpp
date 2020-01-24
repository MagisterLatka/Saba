#include "pch.h"
#include <Saba.h>
#include <imgui.h>

class ExampleLayer : public Saba::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer") {}
	~ExampleLayer() {}

	virtual void OnAttach() override
	{
	}
	virtual void OnDetach() override
	{
	}
	virtual void OnEvent(Saba::Event& event) override
	{
	}
	virtual void OnUpdate() override
	{
		glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, 1.0f);
	}
	virtual void OnImGuiRender() override
	{
		ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
		bool vsync = Saba::Application::Get()->GetWindow()->IsVSync();
		ImGui::Checkbox("Set VSync", &vsync);
		Saba::Application::Get()->GetWindow()->SetVSync(vsync);
		ImGui::ColorEdit3("Clear Color", &m_ClearColor[0]);
	}
private:
	glm::vec3 m_ClearColor = { 0.2f, 0.3f, 0.8f };
};

class Game : public Saba::Application
{
public:
	Game()
	{
		PushLayer(new ExampleLayer);
	}

	~Game()
	{

	}
};
Saba::Application* Saba::CreateApplication()
{
	return new Game();
}
