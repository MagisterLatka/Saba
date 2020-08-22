#include <pch.h>
#include <Saba.h>
#include "ExampleLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

ExampleLayer::ExampleLayer()
	: Saba::Layer("ExampleLayer"), m_ParticleSystem(25000)
{
}
ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnAttach()
{
	Saba::TextureData texData;
	texData.Filepath = "assets/textures/checkerboard.png";
	texData.MagnificationFilter = Saba::Filter::Nearest;
	Saba::TextureManager::Add2D("checkerboard", texData);


	Saba::Ref<Saba::Shader> shader2D = Saba::Shader::Create("assets/shaders/2D.glsl");
	Saba::ShaderManager::Add("2D", shader2D);
	shader2D->Bind();
	int texIDs[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
	};
	shader2D->SetUniformInt1v("u_Textures", texIDs, 32);


	Saba::ShaderManager::Add("particle", "assets/shaders/particle.glsl");

	Saba::FramebufferSpec fbSpec;
	fbSpec.Width = Saba::Application::Get().GetWindow().GetWidth();
	fbSpec.Height = Saba::Application::Get().GetWindow().GetHeight();
	fbSpec.Format = Saba::FramebufferFormat::RGBA8;
	fbSpec.Attachments = static_cast<Saba::FramebufferAttachments>(Saba::FramebufferAttachments::ColorAttachment0 | Saba::FramebufferAttachments::DepthAttachment);
	m_FBO = Saba::Framebuffer::Create(fbSpec);


	m_Particle.ColorBegin = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f);
	m_Particle.ColorEnd = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Particle.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Particle.SizeBegin = 0.5f;
	m_Particle.SizeEnd = 0.1f;
	m_Particle.SizeVariation = 0.05f;
	m_Particle.Velocity = glm::vec2(0.0f, 0.0f);
	m_Particle.VelocityVariation = glm::vec2(3.0f, 3.0f);


	Saba::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });


	for (float y = 0.0f; y < 10.0f; y += m_QuadFrequency)
	{
		for (float x = 0.0f; x < 10.0f; x += m_QuadFrequency)
		{
			m_Scene.CreateEntity().AddComponent<Saba::SpriteComponent>(glm::vec2(x - 5.0f + m_QuadFrequency / 2.0f, y - 5.0f + m_QuadFrequency / 2.0f),
																 glm::vec2(m_QuadFrequency * 0.6f, m_QuadFrequency * 0.6f), glm::vec4(x / 10.0f, y / 10.0f, 1.0f, 1.0f));
		}
	}

	m_Camera = m_Scene.CreateEntity("Camera");
	auto& camera = m_Camera.AddComponent<Saba::CameraComponent>().Camera;
	camera.SetOrthographicSize(10.0f);
	camera.SetViewportSize(Saba::Application::Get().GetWindow().GetWidth(), Saba::Application::Get().GetWindow().GetHeight());

	class CameraController : public Saba::ScriptableEntity
	{
	public:
		virtual void OnEvent(Saba::Event& event) override
		{
			Saba::Dispatcher dispatcher(event);
			dispatcher.Dispatch<Saba::MouseScrolledEvent>(SB_BIND_EVENT_FUNC(CameraController::OnScrollEvent));
		}
		virtual void OnUpdate(Saba::Timestep ts) override
		{
			auto& transform = GetComponent<Saba::TransformComponent>().Transform;

			if (p_RotationEnabled)
			{
				float rotation = 0.0f;
				if (Saba::Input::IsKeyPressed(Saba::KeyCode::Q))
					rotation += m_CameraRotationSpeed * (float)ts;
				if (Saba::Input::IsKeyPressed(Saba::KeyCode::E))
					rotation -= m_CameraRotationSpeed * (float)ts;

				if (rotation != 0.0f)
				{
					transform = glm::rotate(transform, glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
				}
			}

			glm::vec2 translate(0.0f);
			if (Saba::Input::IsKeyPressed(Saba::KeyCode::W))
				translate.y += m_CameraMovementSpeed * (float)ts;
			if (Saba::Input::IsKeyPressed(Saba::KeyCode::S))
				translate.y -= m_CameraMovementSpeed * (float)ts;
			if (Saba::Input::IsKeyPressed(Saba::KeyCode::A))
				translate.x -= m_CameraMovementSpeed * (float)ts;
			if (Saba::Input::IsKeyPressed(Saba::KeyCode::D))
				translate.x += m_CameraMovementSpeed * (float)ts;
			if (translate != glm::vec2(0.0f))
			{
				transform = glm::translate(transform, glm::vec3(translate, 0.0f));
			}
		}
	private:
		bool OnScrollEvent(Saba::MouseScrolledEvent& event)
		{
			m_Zoom -= event.GetYOffset() * 0.5f;
			m_Zoom = glm::max(m_Zoom, 0.5f);
			GetComponent<Saba::CameraComponent>().Camera.SetOrthographicSize(m_Zoom);
			m_CameraMovementSpeed = m_Zoom * 3.0f;
			return false;
		}
	public:
		bool p_RotationEnabled = true;
	private:
		float m_Zoom = 10.0f;

		float m_CameraMovementSpeed = 3.0f;
		float m_CameraRotationSpeed = 90.0f;
	};

	m_Camera.AddComponent<Saba::NativeScriptComponent>().Bind<CameraController>();
}
void ExampleLayer::OnDetach()
{
}
void ExampleLayer::OnEvent(Saba::Event& event)
{
	Saba::Dispatcher dispatcher(event);
	dispatcher.Dispatch<Saba::WindowResizeEvent>(SB_BIND_EVENT_FUNC(ExampleLayer::OnWindowResize));
	m_Scene.OnEvent(event);
}
void ExampleLayer::OnUpdate(Saba::Timestep ts)
{
	Saba::Renderer2D::ResetStats();

	m_FBO->Bind();
	Saba::RenderCommand::Clear();

	m_Scene.OnUpdate(ts, Saba::ShaderManager::Get("2D"));

	Saba::Renderer2D::Flush();

	if (m_EnableParticles)
	{
		if (Saba::Input::IsMouseButtonPressed(SB_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Saba::Input::GetMousePos();
			auto width = Saba::Application::Get().GetWindow().GetWidth();
			auto height = Saba::Application::Get().GetWindow().GetHeight();

			glm::vec2 bounds = { m_Camera.GetComponent<Saba::CameraComponent>().Camera.GetWidth(), m_Camera.GetComponent<Saba::CameraComponent>().Camera.GetHeight() };
			x = x / width * bounds.x - bounds.x * 0.5f;
			y = bounds.y * 0.5f - y / height * bounds.y;

			m_Particle.Position = { x, y, 0.1f };
			m_Particle.Position = m_Camera.GetComponent<Saba::TransformComponent>().Transform * glm::vec4(m_Particle.Position, 1.0f);
			for (int i = 0; i < 1000 * ts; i++)
				m_ParticleSystem.Emit(m_Particle);
		}
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(Saba::ShaderManager::Get("particle"), m_Camera.GetComponent<Saba::CameraComponent>().Camera, m_Camera.GetComponent<Saba::TransformComponent>());

	m_FBO->Unbind();
	Saba::RenderCommand::Clear();

	static constexpr glm::mat4 identity(1.0f);
	Saba::Renderer2D::BeginScene(Saba::ShaderManager::Get("2D"), identity);
	Saba::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 2.0f, 2.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, Saba::Texture2D::Create(m_FBO->GetAttachmentID(0)));
	Saba::Renderer2D::Flush();
}
void ExampleLayer::OnImGuiRender()
{
	ImGui::Text("Framerate: %.1f (frame time: %.3fms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	bool vsync = Saba::Application::Get().GetWindow().IsVSync();
	ImGui::Checkbox("Set VSync", &vsync);
	if (vsync != Saba::Application::Get().GetWindow().IsVSync())
		Saba::Application::Get().GetWindow().SetVSync(vsync);

	ImGui::Text("");

	ImGui::Text("Renderer2D stats:");
	ImGui::Text("\tQuads: %d", Saba::Renderer2D::GetStats().quadCount);
	ImGui::Text("\tDraw calls: %d", Saba::Renderer2D::GetStats().drawCalls);

	ImGui::Text("");

	ImGui::Checkbox("Enable Particles", &m_EnableParticles);
	ImGui::ColorEdit4("ColorBegin", &m_Particle.ColorBegin[0]);
	ImGui::ColorEdit4("ColorEnd", &m_Particle.ColorEnd[0]);
	ImGui::SliderFloat("LifeTime", &m_Particle.LifeTime, 0.1f, 10.0f);
}
bool ExampleLayer::OnWindowResize(Saba::WindowResizeEvent& e)
{
	m_FBO->Resize(e.GetXSize(), e.GetYSize());
	m_Scene.OnViewportResize(e.GetXSize(), e.GetYSize());
	return false;
}
