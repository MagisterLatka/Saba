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

		m_Window = std::make_unique<Window>("Game", 1280, 720);
	}
	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

}
