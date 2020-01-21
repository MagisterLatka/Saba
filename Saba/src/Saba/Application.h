#pragma once

#include "Window.h"
#include "Events\WindowEvent.h"

namespace Saba {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		static Application* Get() { return s_Application; }
	private:
		void OnEvent(Event& event);
		bool OnClose(WindowCloseEvent& event);
	private:
		static Application* s_Application;

		bool m_Running = true;

		std::unique_ptr<Window> m_Window;
	};

	Application* CreateApplication();

}
