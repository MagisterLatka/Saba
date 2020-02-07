#include "pch.h"
#include <Saba.h>
#include "ExampleLayer.h"
#include <GLAD\include\glad\glad.h>
#include <imgui/imgui.h>

#include <GLFW\include\GLFW\glfw3.h>

ExampleLayer::ExampleLayer()
	: Saba::Layer("ExampleLayer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
{
}
ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnAttach()
{
	m_VAO = Saba::VertexArray::Create();


	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	Saba::Ref<Saba::VertexBuffer> vbo = Saba::VertexBuffer::Create(vertices, sizeof(vertices));
	vbo->SetLayout({
		{"i_Pos", Saba::ShaderDataType::Float3}
				   });
	m_VAO->AddVertexBuffer(vbo);


	uint indices[] = { 0, 1, 2 };
	Saba::Ref<Saba::IndexBuffer> ibo = Saba::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));
	m_VAO->SetIndexBuffer(ibo);


	std::string vertexSrc = R"(
		#version 330 core

		layout(location = 0) in vec3 i_Pos;

		uniform mat4 u_ViewProjMat;

		void main()
		{
			gl_Position = u_ViewProjMat * vec4(i_Pos, 1.0f);
		}
	)";

	std::string fragmentSrc = R"(
		#version 330 core

		out vec4 o_Color;

		void main()
		{
			o_Color = vec4(0.2f, 0.3f, 0.8f, 1.0f);
		}
	)";

	m_Shader = Saba::Shader::Create(vertexSrc.c_str(), fragmentSrc.c_str());


	m_Particle.ColorBegin = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f);
	m_Particle.ColorEnd = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Particle.Position = glm::vec2(0.0f, 0.0f);
	m_Particle.SizeBegin = 0.1f;
	m_Particle.SizeEnd = 0.01f;
	m_Particle.SizeVariation = 0.05f;
	m_Particle.Velocity = glm::vec2(0.0f, 0.0f);
	m_Particle.VelocityVariation = glm::vec2(1.0f, 1.0f);


	Saba::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void ExampleLayer::OnDetach()
{
}
void ExampleLayer::OnEvent(Saba::Event& event)
{
}
void ExampleLayer::OnUpdate(Saba::Timestep ts)
{
	if (Saba::Input::IsKeyPressed(GLFW_KEY_D))		m_CameraPos.x += m_CameraVelocity * (float)ts;
	else if (Saba::Input::IsKeyPressed(GLFW_KEY_A)) m_CameraPos.x -= m_CameraVelocity * (float)ts;

	if (Saba::Input::IsKeyPressed(GLFW_KEY_W))		m_CameraPos.y += m_CameraVelocity * (float)ts;
	else if (Saba::Input::IsKeyPressed(GLFW_KEY_S)) m_CameraPos.y -= m_CameraVelocity * (float)ts;

	if (Saba::Input::IsKeyPressed(GLFW_KEY_E))		m_CameraRotation += m_CameraRotationSpeed * (float)ts;
	else if (Saba::Input::IsKeyPressed(GLFW_KEY_Q)) m_CameraRotation -= m_CameraRotationSpeed * (float)ts;

	m_Camera.SetPosition(m_CameraPos);
	m_Camera.SetRotation(m_CameraRotation);

	Saba::RenderCommand::Clear();
	Saba::Renderer::BeginScene(m_Camera);

	Saba::Renderer::Submit(m_Shader, m_VAO);

	if (Saba::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Saba::Input::GetMousePos();
		auto width = Saba::Application::Get()->GetWindow()->GetWidth();
		auto height = Saba::Application::Get()->GetWindow()->GetHeight();
		x = x / width * 3.2f - 1.6f;
		y = 0.9f - y / height * 1.8f;

		m_Particle.Position = { x + m_CameraPos.x, y + m_CameraPos.y };
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
	ImGui::DragFloat("LifeTime", &m_Particle.LifeTime, 0.1f, 0.1f, 10.0f);
}
