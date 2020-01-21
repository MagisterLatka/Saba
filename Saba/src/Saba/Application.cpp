#include "pch.h"
#include "Application.h"
#include "Log.h"

#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"

namespace Saba {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Application = nullptr;

	Application::Application()
	{
		SB_CORE_ASSERT(!s_Application, "Application already exist!");
		s_Application = this;
		SB_CORE_INFO("Created application!");

		m_Window = std::make_unique<Window>("Game", 1280, 720);
		m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));
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

	void Application::OnEvent(Event& event)
	{
		Dispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnClose));

		SB_CORE_TRACE("{0}", event);
	}
	bool Application::OnClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return false;
	}

}
