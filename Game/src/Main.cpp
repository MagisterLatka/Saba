#include "pch.h"
#include <Saba.h>

class Game : public Saba::Application
{
public:
	Game()
	{

	}

	~Game()
	{

	}
};
Saba::Application* Saba::CreateApplication()
{
	return new Game();
}
