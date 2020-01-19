#include "Application.h"
#include "Log.h"

namespace Saba {

	Application* Application::s_Application = nullptr;

	Application::Application()
	{
		SB_CORE_ASSERT(!s_Application, "Application already exist!");
		s_Application = this;
		SB_CORE_INFO("Created application!");
	}
	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (true)
		{

		}
	}

}
