#pragma once

#include "Window.h"

namespace Saba {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		static Application* Get() { return s_Application; }
	private:
		static Application* s_Application;

		bool m_Running = true;

		std::unique_ptr<Window> m_Window;
	};

	Application* CreateApplication();

}
