#pragma once

#include "Saba/Core.h"
#include "Saba/Window.h"
#include "Saba/Events/WindowEvent.h"
#include "Saba/Layers/LayerStack.h"
#include "Saba/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Saba {

	class Application
	{
	public:
		Application(const std::string& name = "Saba App");
		virtual ~Application();

		void Close();
		Window& GetWindow() { return *m_Window; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Application; }
	private:
		void Run();
		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
	private:
		Scope<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		long long m_LastFrameTime = 0;

		Scope<LayerStack> m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		static Application* s_Application;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();

}
