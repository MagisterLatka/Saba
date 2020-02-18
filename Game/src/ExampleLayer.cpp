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
	m_VAO = Saba::VertexArray::Create();


	float vertices[] = {
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.5f, 1.0f
	};
	Saba::Ref<Saba::VertexBuffer> vbo = Saba::VertexBuffer::Create(vertices, sizeof(vertices));
	vbo->SetLayout({
		{"i_Pos", Saba::ShaderDataType::Float3},
		{"i_UV", Saba::ShaderDataType::Float2}
	});
	m_VAO->AddVertexBuffer(vbo);


	uint32_t indices[] = { 0, 1, 2 };
	Saba::Ref<Saba::IndexBuffer> ibo = Saba::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VAO->SetIndexBuffer(ibo);


	m_Shader = Saba::Shader::Create("assets/shaders/basic.glsl");
	m_Shader->Bind();
	m_Shader->SetUniformInt1("u_Tex", 0);
	m_Texture = Saba::Texture2D::Create("assets/textures/checkerboard.png");


	m_Particle.ColorBegin = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f);
	m_Particle.ColorEnd = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Particle.Position = glm::vec2(0.0f, 0.0f);
	m_Particle.SizeBegin = 0.1f;
	m_Particle.SizeEnd = 0.01f;
	m_Particle.SizeVariation = 0.05f;
	m_Particle.Velocity = glm::vec2(0.0f, 0.0f);
	m_Particle.VelocityVariation = glm::vec2(1.0f, 1.0f);


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
	Saba::Renderer::BeginScene(m_CameraControler.GetCamera());

	m_Texture->Bind();
	Saba::Renderer::Submit(m_Shader, m_VAO);

	if (Saba::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Saba::Input::GetMousePos();
		auto width = Saba::Application::Get()->GetWindow()->GetWidth();
		auto height = Saba::Application::Get()->GetWindow()->GetHeight();
		x = x / width * 3.2f - 1.6f;
		y = 0.9f - y / height * 1.8f;

		m_Particle.Position = { x + m_CameraControler.GetCamera().GetPosition().x, y + m_CameraControler.GetCamera().GetPosition().y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender();

	Saba::Renderer::EndScene();
}
void ExampleLayer::OnImGuiRender()
{
	ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
	bool vsync = Saba::Application::Get()->GetWindow()->IsVSync();
	ImGui::Checkbox("Set VSync", &vsync);
	Saba::Application::Get()->GetWindow()->SetVSync(vsync);

	ImGui::Text("");

	ImGui::ColorEdit4("ColorBegin", &m_Particle.ColorBegin[0]);
	ImGui::ColorEdit4("ColorEnd", &m_Particle.ColorEnd[0]);
	ImGui::SliderFloat("LifeTime", &m_Particle.LifeTime, 0.1f, 10.0f);
}
