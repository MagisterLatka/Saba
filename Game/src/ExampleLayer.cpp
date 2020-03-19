#include "pch.h"
#include <Saba.h>
#include "ExampleLayer.h"
#include <GLAD\include\glad\glad.h>
#include <imgui/imgui.h>

#include <GLFW\include\GLFW\glfw3.h>

ExampleLayer::ExampleLayer()
	: Saba::Layer("ExampleLayer"), m_CameraControler(16.0f / 9.0f)
{
}
ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnAttach()
{
	m_Texture = Saba::Texture2D::Create("assets/textures/checkerboard.png");


	m_Particle.ColorBegin = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f);
	m_Particle.ColorEnd = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Particle.Position = glm::vec2(0.0f, 0.0f);
	m_Particle.SizeBegin = 0.5f;
	m_Particle.SizeEnd = 0.1f;
	m_Particle.SizeVariation = 0.05f;
	m_Particle.Velocity = glm::vec2(0.0f, 0.0f);
	m_Particle.VelocityVariation = glm::vec2(3.0f, 3.0f);


	Saba::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
}
void ExampleLayer::OnDetach()
{
}
void ExampleLayer::OnEvent(Saba::Event& event)
{
	m_CameraControler.OnEvent(event);
}
void ExampleLayer::OnUpdate(Saba::Timestep ts)
{
	m_CameraControler.OnUpdate(ts);

	Saba::RenderCommand::Clear();

	Saba::Renderer2D::BeginScene(m_CameraControler.GetCamera().GetViewProjectionMat());

	Saba::Renderer2D::DrawQuad({ -5.0f, -5.0f }, { 10.0f, 10.0f }, m_Texture);

	for (float y = 0.0f; y < 10.0; y += 0.1f)
	{
		for (float x = 0.0f; x < 10.0f; x += 0.1f)
		{
			Saba::Renderer2D::DrawQuad({ x - 5.0f, y - 5.0f }, { 0.09f, 0.09f }, {x / 10.0f, y / 10.0f, 1.0f, 1.0f});
		}
	}

	Saba::Renderer2D::EndScene();
	Saba::Renderer2D::Flush();

	Saba::Renderer::BeginScene(m_CameraControler.GetCamera());

	if (m_EnableParticles)
	{
		if (Saba::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Saba::Input::GetMousePos();
			auto width = Saba::Application::Get()->GetWindow()->GetWidth();
			auto height = Saba::Application::Get()->GetWindow()->GetHeight();

			glm::vec2 bounds = { m_CameraControler.GetWidth(), m_CameraControler.GetHeight() };
			glm::vec2 pos = m_CameraControler.GetCamera().GetPosition();
			x = x / width * bounds.x - bounds.x * 0.5f;
			y = bounds.y * 0.5f - y / height * bounds.y;

			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender();
	}

	Saba::Renderer::EndScene();
}
void ExampleLayer::OnImGuiRender()
{
	ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
	bool vsync = Saba::Application::Get()->GetWindow()->IsVSync();
	ImGui::Checkbox("Set VSync", &vsync);
	Saba::Application::Get()->GetWindow()->SetVSync(vsync);

	ImGui::Text("");

	ImGui::Checkbox("Enable Particles", &m_EnableParticles);
	ImGui::ColorEdit4("ColorBegin", &m_Particle.ColorBegin[0]);
	ImGui::ColorEdit4("ColorEnd", &m_Particle.ColorEnd[0]);
	ImGui::SliderFloat("LifeTime", &m_Particle.LifeTime, 0.1f, 10.0f);
}
