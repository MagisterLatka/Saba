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
	Saba::Renderer2D::SetShader(shader2D);


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
	auto& camera = m_Camera.AddComponent<Saba::CameraComponent>(Saba::SceneCamera::Type::Orthographic).Camera;
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
			auto& tc = GetComponent<Saba::TransformComponent>();

			if (p_RotationEnabled)
			{
				float& rotation = tc.EulerAngles.z;
				if (Saba::Input::IsKeyPressed(Saba::KeyCode::Q))
					rotation += m_CameraRotationSpeed * (float)ts;
				if (Saba::Input::IsKeyPressed(Saba::KeyCode::E))
					rotation -= m_CameraRotationSpeed * (float)ts;
			}

			glm::vec3& translate = tc.Pos;
			if (Saba::Input::IsKeyPressed(Saba::KeyCode::W))
				translate.y += m_CameraMovementSpeed * (float)ts;
			if (Saba::Input::IsKeyPressed(Saba::KeyCode::S))
				translate.y -= m_CameraMovementSpeed * (float)ts;
			if (Saba::Input::IsKeyPressed(Saba::KeyCode::A))
				translate.x -= m_CameraMovementSpeed * (float)ts;
			if (Saba::Input::IsKeyPressed(Saba::KeyCode::D))
				translate.x += m_CameraMovementSpeed * (float)ts;
		}
	private:
		bool OnScrollEvent(Saba::MouseScrolledEvent& event)
		{
			auto& camera = GetComponent<Saba::CameraComponent>().Camera;
			float zoom = camera.GetOrthographicSize();
			zoom -= event.GetYOffset() * 0.5f;
			zoom = glm::max(zoom, 0.5f);
			GetComponent<Saba::CameraComponent>().Camera.SetOrthographicSize(zoom);
			m_CameraMovementSpeed = zoom * 0.5f;
			return false;
		}
	public:
		bool p_RotationEnabled = true;
	private:
		float m_CameraMovementSpeed = 5.0f;
		float m_CameraRotationSpeed = glm::half_pi<float>();
	};

	m_Camera.AddComponent<Saba::NativeScriptComponent>().Bind<CameraController>();

	m_Scene.OnStart();
}
void ExampleLayer::OnDetach()
{
	m_Scene.OnEnd();
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

	m_Scene.OnUpdate(ts);

	Saba::Renderer2D::Flush();

	if (m_EnableParticles)
	{
		if (Saba::Input::IsMouseButtonPressed(SB_MOUSE_BUTTON_LEFT))
		{
			glm::vec2 pos = Saba::Input::GetMousePos();
			auto width = Saba::Application::Get().GetWindow().GetWidth();
			auto height = Saba::Application::Get().GetWindow().GetHeight();

			glm::vec2 bounds = { m_Camera.GetComponent<Saba::CameraComponent>().Camera.GetWidth(), m_Camera.GetComponent<Saba::CameraComponent>().Camera.GetHeight() };
			pos.x = pos.x / width * bounds.x - bounds.x * 0.5f;
			pos.y = bounds.y * 0.5f - pos.y / height * bounds.y;

			m_Particle.Position = { pos.x, pos.y, 0.1f };
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
	Saba::Renderer2D::BeginScene(identity);
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
