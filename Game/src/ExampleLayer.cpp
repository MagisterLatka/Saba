#include <pch.h>
#include <Saba.h>
#include "ExampleLayer.h"

#include <imgui/imgui.h>
#include <GLFW\include\GLFW\glfw3.h>
#include <glm\glm\gtc\matrix_transform.hpp>


struct ViewProjMatBufferData
{
	glm::mat4 viewProjMat;
};
struct SceneBufferData
{
	glm::vec3 viewPos;
	Saba::Light::LightData lights[10];
};

constexpr int texIDs[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

ExampleLayer::ExampleLayer()
	: Saba::Layer("ExampleLayer"), m_CameraControler(16.0f / 9.0f, glm::radians(90.0f), 0.01f, 10.0f)
{
}
ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnAttach()
{
	Saba::TextFile shaders("assets/shaders/shaders.txt", Saba::TextFile::Input | Saba::TextFile::Binary);
	auto& line = Saba::TextReader::GetLine(shaders);
	do
	{
		auto name = Saba::TextReader::GetTillChar(line.first, ',');
		if (!name)
		{
			SB_ERROR("Shaders list syntax error");
		}
		std::string shaderName(name->data(), name->size());

		auto path = Saba::TextReader::GetTillOneOf(line.first, " \t\r\n\0", line.first.find_first_not_of(" \t", name->size() + 1));
		if (!path)
		{
			SB_ERROR("Shaders list syntax error");
		}
		std::string filepath(path->size() + 16, '\0');
		memcpy(&filepath[0], "assets/shaders/", 15);
		memcpy(&filepath[15], path->data(), path->size());
		Saba::ShaderManager::Add(shaderName, filepath);
		line = Saba::TextReader::GetLine(shaders);
	} while (!line.second);

	Saba::Ref<Saba::Shader> shader = Saba::ShaderManager::Get("3d");
	shader->Bind();
	shader->SetUniformInt1v("u_Tex", texIDs, 32);

	shader = Saba::ShaderManager::Get("3dnolight");
	shader->Bind();
	shader->SetUniformInt1v("u_Tex", texIDs, 32);

	shader = Saba::ShaderManager::Get("basic");
	shader->Bind();
	shader->SetUniformInt1("u_Tex", 1);

	Saba::UniformBufferManager::Add("viewProjMat", nullptr, sizeof(ViewProjMatBufferData));
	Saba::UniformBufferManager::Get("viewProjMat")->SetBinding(0);

	Saba::UniformBufferManager::Add("scene", nullptr, sizeof(SceneBufferData));
	Saba::UniformBufferManager::Get("scene")->SetBinding(1);

	Saba::TextureManager::Add2D("brick", "assets/textures/brick.jpg");

	m_Scene.Add(new Saba::Cube({ 0.0f, 0.0f, 2.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, Saba::TextureManager::Get2D("brick")));
	m_Scene.Add(new Saba::Sphere({ 0.0f,  3.0f, 2.0f }, { 1.5f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, Saba::TextureManager::Get2D("brick")));
	m_Scene.Add(new Saba::Sphere({ 0.0f, -1.0f, 0.0f }, { 1.0f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	m_Scene.Add(new Saba::Sphere({ 1.5f,  0.0f, 2.0f }, { 1.0f, 0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f }, Saba::TextureManager::Get2D("brick")));
	m_Scene.Add(new Saba::Sphere({ 0.0f, 1.0f, 2.0f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, Saba::TextureManager::Get2D("brick")));

	constexpr glm::vec3 lightPos = { 0.0f, 1.0f, 0.0f };
	m_Scene.Add(new Saba::Sphere(lightPos, { 0.2f, 0.2f, 0.2f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, false));
	m_Scene.AddLight(new Saba::PointLight(lightPos, 10.0f, { 0.5f, 0.5f, 0.5f }, { 0.8f, 0.8f, 0.8f }));

	constexpr glm::vec3 lampPos = { 0.0f, -2.0f, 2.0f };
	constexpr glm::vec3 lampDir = { 0.0f, 1.0f, 0.0f };
	m_Scene.Add(new Saba::Cube(lampPos, { 0.5f, 0.3f, 0.3f }, lampDir, { 1.0f, 1.0f, 1.0f, 1.0f }, false));
	m_Scene.AddLight(new Saba::SpotLight(lampPos, lampDir, 7.5f, 15.0f, 10.0f, { 0.5f, 0.5f, 0.5f }, { 0.8f, 0.8f, 0.8f }));

	m_Scene.AddLight(new Saba::DirectionalLight({ 0.0f, -1.0f, 0.0f }, { 0.8f, 0.8f, 0.8f }));


	m_SceneFramebufferRenderbuffer = Saba::Renderbuffer::Create(1280, 720, Saba::Renderbuffer::Format::DEPTH);
	m_SceneFramebufferTexture = Saba::Texture2D::Create(1280, 720);
	m_SceneFramebuffer = Saba::Framebuffer::Create();
	m_SceneFramebuffer->Bind();
	m_SceneFramebuffer->AttachTexture(m_SceneFramebufferTexture, Saba::Framebuffer::Attachment::Color0);
	m_SceneFramebuffer->AttachRenderbuffer(m_SceneFramebufferRenderbuffer, Saba::Framebuffer::Attachment::Depth);
	m_SceneFramebuffer->Unbind();


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
	if (Saba::Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		m_CameraControler.SetMovementSpeed(1.0f);
	else if (Saba::Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		m_CameraControler.SetMovementSpeed(10.0f);
	else
		m_CameraControler.SetMovementSpeed(5.0f);

	Saba::RenderCommand::Clear();
	Saba::Renderer3D::ResetStats();

	m_SceneFramebuffer->Bind();
	Saba::RenderCommand::EnableDepthTest();
	Saba::RenderCommand::Clear();
	
	Saba::ShaderManager::Get("3d")->Bind();

	Saba::UniformBufferManager::Get("viewProjMat")->Bind();
	ViewProjMatBufferData data = { m_CameraControler.GetCamera().GetProjectionViewMatrix() };
	Saba::UniformBufferManager::Get("viewProjMat")->SetData(&data, sizeof(ViewProjMatBufferData), 0);

	Saba::UniformBufferManager::Get("scene")->Bind();
	SceneBufferData dataScene = {};
	dataScene.viewPos = m_CameraControler.GetPosition();
	m_Scene.GetLightsData(dataScene.lights);
	Saba::UniformBufferManager::Get("scene")->SetData(&dataScene, sizeof(SceneBufferData), 0);

	Saba::Renderer3D::BeginScene();
	m_Scene.DrawAllLighted();
	Saba::Renderer3D::EndScene();
	Saba::Renderer3D::Flush();

	Saba::ShaderManager::Get("3dnolight")->Bind();
	Saba::Renderer3D::BeginScene();
	m_Scene.DrawAllNotLighted();
	Saba::Renderer3D::EndScene();
	Saba::Renderer3D::Flush();

	Saba::ShaderManager::Get("basic")->Bind();
	m_SceneFramebuffer->Unbind();
	m_SceneFramebufferTexture->Bind(1);
	Saba::RenderCommand::DisableDepthTest();
	Saba::Renderer2D::BeginScene();
	Saba::Renderer2D::DrawQuad({ -1.0f, -1.0f, 0.0f }, { 2.0f, 2.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	Saba::Renderer2D::EndScene();
	Saba::Renderer2D::Flush();
}
void ExampleLayer::OnImGuiRender()
{
	ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
	bool vsync = Saba::Application::Get()->GetWindow()->IsVSync();
	ImGui::Checkbox("Set VSync", &vsync);
	Saba::Application::Get()->GetWindow()->SetVSync(vsync);

	ImGui::Text("");

	ImGui::Text("Renderer3D stats:");

	ImGui::Text("\tTriangles:");
	ImGui::Text("\t\tCount: %d", Saba::Renderer3D::GetStats().triangleCount);
	ImGui::Text("\t\tDraw calls: %d", Saba::Renderer3D::GetStats().drawCallsOnTriangles);

	ImGui::Text("\tQuads:");
	ImGui::Text("\t\tCount: %d", Saba::Renderer3D::GetStats().quadCount);
	ImGui::Text("\t\tDraw calls: %d", Saba::Renderer3D::GetStats().drawCallsOnQuads);

	for (int i = 0; i < Saba::Renderer3D::GetStats().modelStats.size(); i++)
	{
		ImGui::Text("\tModel (id: %d):", i);
		ImGui::Text("\t\tVertices count: %d", Saba::Renderer3D::GetStats().modelStats[i].verticesCount);
		ImGui::Text("\t\tIndices count: %d", Saba::Renderer3D::GetStats().modelStats[i].indicesCount);
		ImGui::Text("\t\tTimes drawed: %d", Saba::Renderer3D::GetStats().modelStats[i].timesDrawed);
	}

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
