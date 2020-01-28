#pragma once

#include "Window.h"
#include "Events\WindowEvent.h"
#include "Layers\LayerStack.h"
#include "ImGui\ImGuiLayer.h"

int main(int argc, char** argv);

namespace Saba {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		static Application* Get() { return s_Application; }
	private:
		void Run();
		void OnEvent(Event& event);
		bool OnClose(WindowCloseEvent& event);
	private:
		bool m_Running = true;
		ImGuiLayer* m_ImGuiLayer;

		std::unique_ptr<LayerStack> m_LayerStack;
		std::unique_ptr<Window> m_Window;

		static Application* s_Application;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();

}
