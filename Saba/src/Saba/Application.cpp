#include "pch.h"
#include "Application.h"
#include "Log.h"

#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"
#include "Events\WindowEvent.h"

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
