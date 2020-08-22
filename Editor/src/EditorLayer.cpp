#include <pch.h>
#include <Saba.h>
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Saba {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_ParticleSystem(25000)
	{}
	EditorLayer::~EditorLayer()
	{}

	void EditorLayer::OnAttach()
	{
		FramebufferSpec fbSpec;
		fbSpec.Width = Application::Get().GetWindow().GetWidth();
		fbSpec.Height = Application::Get().GetWindow().GetHeight();
		fbSpec.Format = FramebufferFormat::RGBA8;
		fbSpec.Attachments = static_cast<FramebufferAttachments>(FramebufferAttachments::ColorAttachment0 | FramebufferAttachments::DepthAttachment);
		m_FBO = Framebuffer::Create(fbSpec);


		TextureData texData;
		texData.Filepath = "assets/textures/checkerboard.png";
		texData.MagnificationFilter = Filter::Nearest;
		TextureManager::Add2D("checkerboard", texData);


		Ref<Shader> shader2D = Shader::Create("assets/shaders/2D.glsl");
		ShaderManager::Add("2D", shader2D);
		shader2D->Bind();
		int texIDs[] = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
		};
		shader2D->SetUniformInt1v("u_Textures", texIDs, 32);


		ShaderManager::Add("particle", "assets/shaders/particle.glsl");


		m_Particle.ColorBegin = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f);
		m_Particle.ColorEnd = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		m_Particle.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Particle.SizeBegin = 0.5f;
		m_Particle.SizeEnd = 0.1f;
		m_Particle.SizeVariation = 0.05f;
		m_Particle.Velocity = glm::vec2(0.0f, 0.0f);
		m_Particle.VelocityVariation = glm::vec2(3.0f, 3.0f);


		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });


		m_ColoredQuad = m_Scene.CreateEntity("Colored Quad");
		m_ColoredQuad.AddComponent<SpriteComponent>(glm::vec3(3.0f, 0.0f, 0.3f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		for (float y = 0.0f; y < 10.0f; y += m_QuadFrequency)
		{
			for (float x = 0.0f; x < 10.0f; x += m_QuadFrequency)
			{
				m_Scene.CreateEntity().AddComponent<SpriteComponent>(glm::vec2(x - 5.0f + m_QuadFrequency / 2.0f, y - 5.0f + m_QuadFrequency / 2.0f),
																	 glm::vec2(m_QuadFrequency * 0.6f, m_QuadFrequency * 0.6f), glm::vec4(x / 10.0f, y / 10.0f, 1.0f, 1.0f));
			}
		}

		m_Camera = m_Scene.CreateEntity("Camera");
		m_Camera.AddComponent<CameraComponent>().Camera.SetOrthographicSize(10.0f);

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnEvent(Event& event) override
			{
				Dispatcher dispatcher(event);
				dispatcher.Dispatch<MouseScrolledEvent>(SB_BIND_EVENT_FUNC(CameraController::OnScrollEvent));
			}
			virtual void OnUpdate(Timestep ts) override
			{
				auto& transform = GetComponent<TransformComponent>().Transform;

				if (p_RotationEnabled)
				{
					float rotation = 0.0f;
					if (Input::IsKeyPressed(KeyCode::Q))
						rotation += m_CameraRotationSpeed * (float)ts;
					if (Input::IsKeyPressed(KeyCode::E))
						rotation -= m_CameraRotationSpeed * (float)ts;

					if (rotation != 0.0f)
					{
						transform = glm::rotate(transform, glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
					}
				}

				glm::vec2 translate(0.0f);
				if (Input::IsKeyPressed(KeyCode::W))
					translate.y += m_CameraMovementSpeed * (float)ts;
				if (Input::IsKeyPressed(KeyCode::S))
					translate.y -= m_CameraMovementSpeed * (float)ts;
				if (Input::IsKeyPressed(KeyCode::A))
					translate.x -= m_CameraMovementSpeed * (float)ts;
				if (Input::IsKeyPressed(KeyCode::D))
					translate.x += m_CameraMovementSpeed * (float)ts;
				if (translate != glm::vec2(0.0f))
				{
					transform = glm::translate(transform, glm::vec3(translate, 0.0f));
				}
			}
		private:
			bool OnScrollEvent(MouseScrolledEvent& event)
			{
				m_Zoom -= event.GetYOffset() * 0.5f;
				m_Zoom = glm::max(m_Zoom, 0.5f);
				GetComponent<CameraComponent>().Camera.SetOrthographicSize(m_Zoom);
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

		m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}
	void EditorLayer::OnDetach()
	{}
	void EditorLayer::OnEvent(Event& event)
	{
		m_Scene.OnEvent(event);
	}
	void EditorLayer::OnUpdate(Timestep ts)
	{
		if (auto& spec = m_FBO->GetSpecification(); m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FBO->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Scene.OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();
		m_FBO->Bind();
		RenderCommand::Clear();

		m_Scene.OnUpdate(ts, ShaderManager::Get("2D"));

		Renderer2D::Flush();

		if (m_EnableParticles)
		{
			if (Input::IsMouseButtonPressed(SB_MOUSE_BUTTON_LEFT))
			{
				auto [x, y] = Input::GetMousePos();

				glm::vec2 bounds = { m_Camera.GetComponent<CameraComponent>().Camera.GetWidth(), m_Camera.GetComponent<CameraComponent>().Camera.GetHeight() };
				x = (x - m_ViewportPos.x) / m_ViewportSize.x * bounds.x - bounds.x * 0.5f;
				y = bounds.y * 0.5f - (y - m_ViewportPos.y) / m_ViewportSize.y * bounds.y;

				m_Particle.Position = { x, y, 0.1f };
				m_Particle.Position = m_Camera.GetComponent<TransformComponent>().Transform * glm::vec4(m_Particle.Position, 1.0f);
				for (int i = 0; i < 1000 * ts; i++)
					m_ParticleSystem.Emit(m_Particle);
			}
		}
		
		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(ShaderManager::Get("particle"), m_Camera.GetComponent<CameraComponent>().Camera, m_Camera.GetComponent<TransformComponent>());

		m_FBO->Unbind();
	}
	void EditorLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool optFullscreenPersistant = true;
		bool optFullScreen = optFullscreenPersistant;
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (optFullScreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			windowFlags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Dock", &dockspaceOpen, windowFlags);
		ImGui::PopStyleVar();

		if (optFullScreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceID = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

			ImGui::Begin("Settings");

				ImGui::Text("Framerate: %.1f (frame time: %.3fms)", io.Framerate, 1000.0f / io.Framerate);
				bool vsync = Application::Get().GetWindow().IsVSync();
				ImGui::Checkbox("Set VSync", &vsync);
				if (vsync != Application::Get().GetWindow().IsVSync())
					Application::Get().GetWindow().SetVSync(vsync);

				ImGui::Separator();

				ImGui::Text("Renderer2D stats:");
				ImGui::Text("\tQuads: %d", Renderer2D::GetStats().quadCount);
				ImGui::Text("\tDraw calls: %d", Renderer2D::GetStats().drawCalls);

				ImGui::Separator();

				ImGui::Checkbox("Enable Particles", &m_EnableParticles);
				ImGui::ColorEdit3("ColorBegin", &m_Particle.ColorBegin[0]);
				ImGui::ColorEdit3("ColorEnd", &m_Particle.ColorEnd[0]);
				ImGui::SliderFloat("LifeTime", &m_Particle.LifeTime, 0.1f, 10.0f);

				ImGui::Separator();

				auto& color = m_ColoredQuad.GetComponent<SpriteComponent>().Color;
				ImGui::ColorEdit3("Quad Color", &color[0]);

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
			ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoTitleBar);

				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				Application::Get().GetImGuiLayer()->BlockEvent(!m_ViewportFocused || !m_ViewportHovered);

				ImVec2 viewportSize = ImGui::GetContentRegionAvail();
				m_ViewportSize = { viewportSize.x, viewportSize.y };
				ImVec2 viewportPos = ImGui::GetWindowPos();
				m_ViewportPos = { viewportPos.x - Application::Get().GetWindow().GetWindowPosX(), viewportPos.y - Application::Get().GetWindow().GetWindowPosY() };

				ImGui::Image(m_FBO->GetAttachmentID(0), viewportSize, { 0.0f, 1.0f }, { 1.0f, 0.0f });

			ImGui::End();
			ImGui::PopStyleVar();

		ImGui::End();
	}
}
