#include <pch.h>
#include <Saba.h>
#include "ExampleLayer.h"

#include <imgui/imgui.h>
#include <GLFW\include\GLFW\glfw3.h>

ExampleLayer::ExampleLayer()
	: Saba::Layer("ExampleLayer"), m_CameraControler(16.0f / 9.0f, glm::radians(90.0f), 0.01f, 10.0f)
{
}
ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnAttach()
{
	Saba::Ref<Saba::Shader> shader = Saba::Shader::Create("assets/shaders/3d.glsl");
	Saba::ShaderManager::Add("3d", shader);
	shader->Bind();
	int texIDs[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
	};
	shader->SetUniformInt1v("u_Tex", texIDs, 32);

	Saba::TextureManager::Add2D("brick", "assets/textures/brick.jpg");

	Saba::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
}
void ExampleLayer::OnDetach()
{
}
void ExampleLayer::OnEvent(Saba::Event& event)
{
	m_CameraControler.OnEvent(event);
	Saba::Dispatcher d(event);
	d.Dispatch<Saba::KeyPressedEvent>(SB_BIND_EVENT_FUNC(ExampleLayer::OnKeyPress));
}
void ExampleLayer::OnUpdate(Saba::Timestep ts)
{
	m_CameraControler.OnUpdate(ts);

	Saba::RenderCommand::Clear();
	Saba::Renderer3D::ResetStats();

	Saba::ShaderManager::Get("3d")->Bind();
	Saba::ShaderManager::Get("3d")->SetUniformMat4("u_ViewProjMat", m_CameraControler.GetCamera().GetProjectionViewMatrix());

	Saba::Renderer3D::BeginScene();
	Saba::Renderer3D::DrawQuad({ glm::vec3(0.0f, 0.0f, 2.0f), { 1.0f, 0.0f, 2.0f }, { 1.0f, 1.0f, 2.0f }, { 0.0f, 1.0f, 2.0f }}, Saba::TextureManager::Get2D("brick"));
	Saba::Renderer3D::DrawQuad({ glm::vec3(1.0f, 0.0f, 2.0f), { 1.0f, 0.0f, 3.0f }, { 1.0f, 1.0f, 3.0f }, { 1.0f, 1.0f, 2.0f }}, Saba::TextureManager::Get2D("brick"));
	Saba::Renderer3D::DrawQuad({ glm::vec3(1.0f, 0.0f, 3.0f), { 0.0f, 0.0f, 3.0f }, { 0.0f, 1.0f, 3.0f }, { 1.0f, 1.0f, 3.0f }}, Saba::TextureManager::Get2D("brick"));
	Saba::Renderer3D::DrawQuad({ glm::vec3(0.0f, 0.0f, 3.0f), { 0.0f, 0.0f, 2.0f }, { 0.0f, 1.0f, 2.0f }, { 0.0f, 1.0f, 3.0f }}, Saba::TextureManager::Get2D("brick"));
	Saba::Renderer3D::DrawQuad({ glm::vec3(0.0f, 0.0f, 2.0f), { 1.0f, 0.0f, 2.0f }, { 1.0f, 0.0f, 3.0f }, { 0.0f, 0.0f, 3.0f }}, Saba::TextureManager::Get2D("brick"));
	Saba::Renderer3D::DrawQuad({ glm::vec3(0.0f, 1.0f, 2.0f), { 1.0f, 1.0f, 2.0f }, { 1.0f, 1.0f, 3.0f }, { 0.0f, 1.0f, 3.0f }}, Saba::TextureManager::Get2D("brick"));
	Saba::Renderer3D::EndScene();
	Saba::Renderer3D::Flush();
}
void ExampleLayer::OnImGuiRender()
{
	ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
	bool vsync = Saba::Application::Get()->GetWindow()->IsVSync();
	ImGui::Checkbox("Set VSync", &vsync);
	Saba::Application::Get()->GetWindow()->SetVSync(vsync);

	ImGui::Text("");

	ImGui::Text("Renderer3D stats:");
	ImGui::Text("\tQuads: %d", Saba::Renderer3D::GetStats().quadCount);
	ImGui::Text("\tDraw calls: %d", Saba::Renderer3D::GetStats().drawCallsOnQuads);

	ImGui::Text("");

	glm::vec3 pos = m_CameraControler.GetPosition();
	ImGui::Text("Position: %.1f, %.1f, %.1f", pos.x, pos.y, pos.z);
	glm::vec3 dir = m_CameraControler.GetCamera().GetDirection();
	ImGui::Text("Rotation: %.1f, %.1f, %.1f", dir.x, dir.y, dir.z);
	auto [x, y] = m_CameraControler.GetRotation();
	ImGui::Text("\t\tyaw = %.1f, pitch = %.1f", x, y);
}

bool ExampleLayer::OnKeyPress(Saba::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == GLFW_KEY_I)
	{
		Saba::Application::Get()->GetWindow()->SetCursor(!Saba::Application::Get()->GetWindow()->IsCursorEnabled());
		m_CameraControler.EnableRotation(!m_CameraControler.IsRotationEnabled());
	}
	if (e.GetKeyCode() == GLFW_KEY_ESCAPE)
		Saba::Application::Get()->Close();

	return false;
}
