#include <pch.h>
#include <Saba.h>
#include <Saba\EntryPoint.h>

#include "ExampleLayer.h"

class Game : public Saba::Application
{
public:
	Game()
		:Application("Example")
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
