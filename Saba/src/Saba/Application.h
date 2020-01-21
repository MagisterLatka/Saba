#pragma once

#include "Window.h"
#include "Events\WindowEvent.h"
#include "Layers\LayerStack.h"

namespace Saba {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application* Get() { return s_Application; }
	private:
		void OnEvent(Event& event);
		bool OnClose(WindowCloseEvent& event);
	private:
		static Application* s_Application;

		bool m_Running = true;

		std::unique_ptr<LayerStack> m_LayerStack;
		std::unique_ptr<Window> m_Window;
	};

	Application* CreateApplication();

}
