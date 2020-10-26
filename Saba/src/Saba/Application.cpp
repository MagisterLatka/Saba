#include "pch.h"
#include "Saba/Application.h"
#include "Saba/Renderer/Renderer.h"

namespace Saba {

	Application* Application::s_Application = nullptr;

	Application::Application(const std::string& name)
	{
		SB_CORE_ASSERT(!s_Application, "Application already exist!");
		s_Application = this;
		SB_CORE_INFO("Created application!");

		m_Window = Window::Create(WindowProps(name, 1600, 900));
		m_Window->SetEventCallback(SB_BIND_EVENT_FUNC(Application::OnEvent));

		Renderer::Init();

		m_LayerStack = MakeScope<LayerStack>();
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application()
	{
		delete m_LayerStack.release();
		Renderer::Shutdown();
	}

	void Application::Run()
	{
		m_LastFrameTime = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
		while (m_Running)
		{
			std::chrono::time_point<std::chrono::high_resolution_clock> timePointNow = std::chrono::high_resolution_clock::now();
			long long now = std::chrono::time_point_cast<std::chrono::microseconds>(timePointNow).time_since_epoch().count();
			Timestep ts = (double)(now - m_LastFrameTime) / 1000000.0;
			m_LastFrameTime = now;

			if (!m_Minimized)
			{
				for (auto layer : *m_LayerStack)
					layer->OnUpdate(ts);

				m_ImGuiLayer->Begin();
				for (auto layer : *m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack->PushLayer(layer);
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack->PushOverlay(overlay);
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& event)
	{
		Dispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(SB_BIND_EVENT_FUNC(Application::OnWindowClose));

		for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); it++)
		{
			if (event.p_Handled)
				break;
			(*it)->OnEvent(event);
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return false;
	}
	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		if (event.GetXSize() == 0 || event.GetYSize() == 0)
		{
			m_Minimized = true;
			return false;
		}

		Renderer::OnWindowResize(event.GetXSize(), event.GetYSize());
		m_Minimized = false;

		return false;
	}

}
