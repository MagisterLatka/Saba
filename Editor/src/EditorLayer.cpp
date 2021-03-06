#include <pch.h>
#include <Saba.h>
#include "EditorLayer.h"

#include "Saba/ImGui/SabaImGui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include "Saba/Scene/SceneSerializer.h"
#include "Saba/Utils/PlatformUtils.h"
#include "Scripts.h"

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


		ShaderManager::Add("particle", "assets/shaders/particle.glsl");


		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });


		m_Scene = MakeRef<Scene>();

		/*float vertices[] = {
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
		m_Scene->CreateEntity("Model").AddComponent<ModelComponent>(MakeRef<Mesh>(vertices, 24, Mesh::MeshType(Mesh::MeshType::Position | Mesh::MeshType::Color), indices, 36));

		auto model = m_Scene->CreateEntity("Model2");
		model.GetComponent<TransformComponent>().Pos.x = -3.0f;
		auto& mesh = (*model.AddComponent<ModelComponent>(MakeRef<Model>("assets/models/box.fbx")).Model)[0];
		mesh->SetAlbedoTex(TextureManager::Get2D("checkerboard"));


		m_Camera = m_Scene->CreateEntity("Camera");
		m_Camera.AddComponent<CameraComponent>(SceneCamera::Type::Perspective).Camera.SetPerspective(glm::half_pi<float>(), 0.1f, 10.0f);
		m_Camera.AddComponent<NativeScriptComponent>().Bind<PerspectiveCameraController>();*/

		m_HierarchyPanel.SetScene(m_Scene);

		m_Scene->OnStart();
	}
	void EditorLayer::OnDetach()
	{
		m_Scene->OnEnd();
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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWindowWidth = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceID = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}
		style.WindowMinSize.x = minWindowWidth;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();
				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveScene();
				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
					SaveAsScene();

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();

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

				ImGui::TextUnformatted("Renderer2D stats:");
				ImGui::Text("\tQuads: %d", Renderer2D::GetStats().quadCount);
				ImGui::Text("\tDraw calls: %d", Renderer2D::GetStats().drawCalls);
				ImGui::Text("\tShader: %s", ShaderManager::GetName(Renderer2D::GetCurrentShader()).c_str());
				ImGui::SameLine();
				if (ImGui::Button("Select shader"))
				{
					std::string filepath = FileDialogs::Open("Shader file (*.glsl)\0*.glsl\0");
					auto [shader, created] = ShaderManager::GetFromFilepath(filepath);
					if (shader)
					{
						Renderer2D::SetShader(shader);
						if (created)
						{
							static int texIDs[] = {
								0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
							};
							shader->Bind();
							if (shader->HasUniform("u_Textures"))
								shader->SetUniformInt1v("u_Textures", texIDs, 32);
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Default"))
					Renderer2D::SetShader();

				ImGui::Separator();

				ImGui::TextUnformatted("Renderer3D stats:");
				ImGui::Text("\tVertices: %d", Renderer3D::GetStats().verticesCount);
				ImGui::Text("\tIndices: %d", Renderer3D::GetStats().indicesCount);
				ImGui::Text("\tDraw calls: %d", Renderer3D::GetStats().drawCalls);
				ImGui::Text("\tShader: %s", ShaderManager::GetName(Renderer3D::GetCurrentShader()).c_str());
				ImGui::SameLine();
				if (ImGui::Button("Select shader"))
				{
					std::string filepath = FileDialogs::Open("Shader file (*.glsl)\0*.glsl\0");
					auto [shader, created] = ShaderManager::GetFromFilepath(filepath);
					if (shader)
					{
						Renderer3D::SetShader(shader);
						if (created)
						{
							static int texIDs[] = {
								0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
							};
							shader->Bind();
							if (shader->HasUniform("u_Textures"))
								shader->SetUniformInt1v("u_Textures", texIDs, 32);
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Default"))
					Renderer3D::SetShader();

			ImGui::End();

			m_HierarchyPanel.OnImGuiRender();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
			ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoTitleBar);

				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				m_Scene->SetViewportFocusState(m_ViewportFocused);
				m_Scene->SetViewportHoverState(m_ViewportHovered);
				Application::Get().GetImGuiLayer()->BlockEvent(!m_ViewportFocused && !m_ViewportHovered);

				ImVec2 viewportSize = ImGui::GetContentRegionAvail();
				m_ViewportSize = { viewportSize.x, viewportSize.y };
				ImVec2 viewportPos = ImGui::GetWindowPos();
				m_ViewportPos = { viewportPos.x - Application::Get().GetWindow().GetWindowPosX(), viewportPos.y - Application::Get().GetWindow().GetWindowPosY() };
				m_Scene->SetViewportPos(m_ViewportPos);

				ImGui::Image(m_FBO->GetAttachmentID(0), viewportSize, { 0.0f, 1.0f }, { 1.0f, 0.0f });

				Entity selected = m_HierarchyPanel.GetSelected();
				if (selected && m_GuimzoType != -1)
				{
					bool hasCamera = false;
					glm::mat4 cameraProjection, cameraView;

					if (selected.HasComponent<SpriteComponent>())
					{
						ImGuizmo::SetOrthographic(true);

						auto& camera = m_Scene->GetPrimary2DCameraEntity();
						if (camera)
						{
							cameraProjection = camera.GetComponent<CameraComponent>().Camera.GetProjection();
							cameraView = glm::inverse(camera.GetComponent<TransformComponent>().Transform);
							hasCamera = true;
						}
					}
					else
					{
						ImGuizmo::SetOrthographic(false);

						auto& camera = m_Scene->GetPrimary3DCameraEntity();
						if (camera)
						{
							cameraProjection = camera.GetComponent<CameraComponent>().Camera.GetProjection();
							cameraView = glm::inverse(camera.GetComponent<TransformComponent>().Transform);
							hasCamera = true;
						}
					}

					if (hasCamera)
					{
						ImGuizmo::SetDrawlist();

						float windowWidth = (float)ImGui::GetWindowWidth();
						float windowHeight = (float)ImGui::GetWindowHeight();
						ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

						auto& tc = selected.GetComponent<TransformComponent>();
						glm::mat4 entityTransform = tc.Transform;
						float a = entityTransform[3][0];

						bool snap = Input::IsKeyPressed(Key::LeftControl);
						float snapValue = 0.5f;
						if (m_GuimzoType == ImGuizmo::OPERATION::ROTATE)
							snapValue = 45.0f;
						float snapValues[3] = { snapValue, snapValue, snapValue };

						ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GuimzoType, ImGuizmo::MODE::LOCAL,
											 glm::value_ptr(entityTransform), nullptr, snap ? snapValues : nullptr);

						if (ImGuizmo::IsUsing())
						{
							glm::vec3 translation, rotation, scale;
							Math::Decompose(entityTransform, translation, rotation, scale);

							glm::vec3 delta = rotation - tc.Orientation;
							tc.Pos = translation;
							tc.Orientation += delta;
							tc.Scale = scale;
						}
					}
				}

			ImGui::End();
			ImGui::PopStyleVar();

		ImGui::End();
	}
	void EditorLayer::OnEvent(Event& event)
	{
		m_Scene->OnEvent(event);

		Dispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(SB_BIND_EVENT_FUNC(EditorLayer::OnKeyPress));
	}
	bool EditorLayer::OnKeyPress(KeyPressedEvent& e)
	{
		if (e.GetRepeatedCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
			case Key::N:
				if (control)
					NewScene();
				break;
			case Key::O:
				if (control)
					OpenScene();
				break;
			case Key::S:
				if (control && !shift)
					SaveScene();
				else if (control && shift)
					SaveAsScene();
				break;

			case Key::Q:
				m_GuimzoType = -1;
				break;
			case Key::W:
				m_GuimzoType = 0;
				break;
			case Key::E:
				m_GuimzoType = 1;
				break;
			case Key::R:
				m_GuimzoType = 2;
				break;
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		m_Scene = MakeRef<Scene>();
		m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_HierarchyPanel.SetScene(m_Scene);
		m_CurrentScene = {};
	}
	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::Open("Saba scene (*.saba)\0*.saba\0");
		if (!filepath.empty())
		{
			m_Scene = MakeRef<Scene>();
			m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_HierarchyPanel.SetScene(m_Scene);

			SceneSerializer serializer(m_Scene);
			serializer.Deserialize(filepath);

			m_CurrentScene = filepath;
		}
		else
			m_CurrentScene = {};
	}
	void EditorLayer::SaveScene()
	{
		if (!m_CurrentScene.empty())
		{
			SceneSerializer serializer(m_Scene);
			serializer.Serialize(m_CurrentScene);
		}
		else SaveAsScene();
	}
	void EditorLayer::SaveAsScene()
	{
		std::string filepath = FileDialogs::Save("Saba scene (*.saba)\0*.saba\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_Scene);
			serializer.Serialize(filepath);
			m_CurrentScene = filepath;
		}
	}
}
