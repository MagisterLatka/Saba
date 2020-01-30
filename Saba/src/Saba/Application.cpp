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

		m_LayerStack = std::make_unique<LayerStack>();

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application()
	{

	}

	void Application::Run()
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		while (m_Running)
		{
			RenderCommand::Clear();

			Saba::Renderer::BeginScene();

			for (auto layer : *m_LayerStack)
				layer->OnUpdate();

			Saba::Renderer::EndScene();

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
