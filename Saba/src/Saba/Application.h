#pragma once

#include "Window.h"
#include "Events\WindowEvent.h"
#include "Layers\LayerStack.h"
#include "ImGui\ImGuiLayer.h"

namespace Saba {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline std::shared_ptr<Window> GetWindow() const { return m_Window; }

		static Application* Get() { return s_Application; }
	private:
		void OnEvent(Event& event);
		bool OnClose(WindowCloseEvent& event);
	private:
		static Application* s_Application;

		bool m_Running = true;
		ImGuiLayer* m_ImGuiLayer;

		std::unique_ptr<LayerStack> m_LayerStack;
		std::shared_ptr<Window> m_Window;
	};

	Application* CreateApplication();

}
