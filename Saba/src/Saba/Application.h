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

		void Close();

		inline const Ref<Window>& GetWindow() const { return m_Window; }

		static Application* Get() { return s_Application; }
	private:
		void Run();
		void OnEvent(Event& event);
		bool OnClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
	private:
		bool m_Running = true;
		bool m_Minimized = false;
		ImGuiLayer* m_ImGuiLayer;

		Scope<LayerStack> m_LayerStack;
		Ref<Window> m_Window;
		long long m_LastFrameTime = 0;

		static Application* s_Application;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();

}
