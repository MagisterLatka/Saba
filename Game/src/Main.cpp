#include "pch.h"
#include <Saba.h>

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
		Saba::Dispatcher d(event);
		d.Dispatch<Saba::KeyPressedEvent>(SB_BIND_EVENT_FUNC(ExampleLayer::OnKeyPressed));
	}
	virtual void OnUpdate() override
	{
	}

	bool OnKeyPressed(Saba::KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == GLFW_KEY_ENTER && e.GetRepeatedCount() == 0)
			SB_TRACE("Pressed ENTER"); //Only Debug mode :D

		return false;
	}
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
