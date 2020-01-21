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
	}
	virtual void OnUpdate() override
	{
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
