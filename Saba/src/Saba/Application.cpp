#include "pch.h"
#include "Application.h"
#include "Renderer\Renderer.h"

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

		m_LayerStack = MakeScope<LayerStack>();

		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application()
	{

	}

	void Application::Run()
	{
		m_LastFrameTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
		while (m_Running)
		{
			std::chrono::time_point<std::chrono::high_resolution_clock> timePointNow = std::chrono::high_resolution_clock::now();
			long long now = std::chrono::time_point_cast<std::chrono::milliseconds>(timePointNow).time_since_epoch().count();
			Timestep ts = (double)(now - m_LastFrameTime) / 1000.0;
			m_LastFrameTime = now;

			for (auto layer : *m_LayerStack)
				layer->OnUpdate(ts);

			m_ImGuiLayer->Begin();
			for (auto layer : *m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

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

	void Application::OnEvent(Event& event)
	{
		Dispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnClose));

		for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); it++)
		{
			(*it)->OnEvent(event);
			if (event.p_Handled)
				break;
		}
	}
	bool Application::OnClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return false;
	}

}
