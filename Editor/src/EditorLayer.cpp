#include <pch.h>
#include <Saba.h>
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Scripts.h"

namespace ImGui {

	static void HelpMarker(const char* desc) //From imgui_demo.cpp
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}

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


		ShaderManager::Add("particle", "assets/shaders/particle.glsl");


		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });


		float vertices[] = {
			 1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,   1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,   1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f, 1.0f,

			-1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,   0.0f, 1.0f, 0.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f, 1.0f,

			-1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,   0.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,   0.0f, 0.0f, 1.0f, 1.0f,

			 1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 0.0f, 1.0f,

			-1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 1.0f, 1.0f,

			 1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 1.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 1.0f, 1.0f,
		};
		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20,
		};
		m_Mesh = MakeRef<Mesh>(vertices, 24, Mesh::MeshType(Mesh::MeshType::Position | Mesh::MeshType::Color), indices, 36);


		m_Scene = MakeRef<Scene>();

		m_Camera = m_Scene->CreateEntity("Camera");
		m_Camera.AddComponent<CameraComponent>(SceneCamera::Type::Perspective).Camera.SetPerspective(glm::half_pi<float>(), 0.1f, 10.0f);
		m_Camera.AddComponent<NativeScriptComponent>().Bind<PerspectiveCameraController>();

		m_HierarchyPanel.SetScene(m_Scene);

		m_Scene->OnStart();
	}
	void EditorLayer::OnDetach()
	{
		m_Scene->OnEnd();
	}
	void EditorLayer::OnEvent(Event& event)
	{
		Dispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(SB_BIND_EVENT_FUNC(EditorLayer::OnKeyPress));
		m_Scene->OnEvent(event);
	}
	bool EditorLayer::OnKeyPress(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::I)
		{
			if (m_Camera.GetComponent<NativeScriptComponent>().GetInstance<PerspectiveCameraController>()->p_EnableRotations)
			{
				m_Camera.GetComponent<NativeScriptComponent>().GetInstance<PerspectiveCameraController>()->p_EnableRotations = false;
				Application::Get().GetWindow().EnableCursor();
				Application::Get().GetImGuiLayer()->EnableMouseEvents();
			}
			else
			{
				m_Camera.GetComponent<NativeScriptComponent>().GetInstance<PerspectiveCameraController>()->p_EnableRotations = true;
				Application::Get().GetWindow().EnableCursor(false);
				Application::Get().GetImGuiLayer()->EnableMouseEvents(false);
			}
		}
		return false;
	}
	void EditorLayer::OnUpdate(Timestep ts)
	{
		if (auto& spec = m_FBO->GetSpecification(); m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FBO->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();
		Renderer3D::ResetStats();

		m_FBO->Bind();
		RenderCommand::Clear();

		m_Scene->OnUpdate(ts);

		Renderer3D::BeginScene(m_Camera.GetComponent<CameraComponent>().Camera, m_Camera.GetComponent<TransformComponent>());
		Renderer3D::DrawMesh(m_Mesh);

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

				ImGui::Text("Renderer3D stats:");
				ImGui::Text("\tVertices: %d", Renderer3D::GetStats().verticesCount);
				ImGui::Text("\tIndices: %d", Renderer3D::GetStats().indicesCount);
				ImGui::Text("\tDraw calls: %d", Renderer3D::GetStats().drawCalls);

				ImGui::Separator();

				bool enable = m_Camera.GetComponent<NativeScriptComponent>().GetInstance<PerspectiveCameraController>()->p_EnableRotations;
				ImGui::Checkbox("Camera rotations enabled", &enable);
				ImGui::SameLine(); ImGui::HelpMarker("Press I key to enable camera rotations");

			ImGui::End();

			m_HierarchyPanel.OnImGuiRender();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
			ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoTitleBar);

				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				m_Scene->SetViewportFocusState(m_ViewportFocused);
				m_Scene->SetViewportHoverState(m_ViewportHovered);
				Application::Get().GetImGuiLayer()->BlockEvent(!m_ViewportFocused || !m_ViewportHovered);

				ImVec2 viewportSize = ImGui::GetContentRegionAvail();
				m_ViewportSize = { viewportSize.x, viewportSize.y };
				ImVec2 viewportPos = ImGui::GetWindowPos();
				m_ViewportPos = { viewportPos.x - Application::Get().GetWindow().GetWindowPosX(), viewportPos.y - Application::Get().GetWindow().GetWindowPosY() };
				m_Scene->SetViewportPos(m_ViewportPos);

				ImGui::Image(m_FBO->GetAttachmentID(0), viewportSize, { 0.0f, 1.0f }, { 1.0f, 0.0f });

			ImGui::End();
			ImGui::PopStyleVar();

		ImGui::End();
	}
}
