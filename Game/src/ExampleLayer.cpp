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
	m_VAO.reset(Saba::VertexArray::Create());


	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	std::shared_ptr<Saba::VertexBuffer> vbo;
	vbo.reset(Saba::VertexBuffer::Create(vertices, sizeof(vertices)));
	vbo->SetLayout({
		{"i_Pos", Saba::ShaderDataType::Float3}
	});
	m_VAO->AddVertexBuffer(vbo);


	uint indices[] = { 0, 1, 2 };
	std::shared_ptr<Saba::IndexBuffer> ibo;
	ibo.reset(Saba::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint)));
	m_VAO->SetVertexBuffer(ibo);

	
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

	m_Shader.reset(Saba::Shader::Create(vertexSrc.c_str(), fragmentSrc.c_str()));
	
	
	Saba::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
}
void ExampleLayer::OnDetach()
{
}
void ExampleLayer::OnEvent(Saba::Event& event)
{
}
void ExampleLayer::OnUpdate()
{
	if (Saba::Input::IsKeyPressed(GLFW_KEY_D))		m_CameraPos.x += 0.1f;
	else if (Saba::Input::IsKeyPressed(GLFW_KEY_A)) m_CameraPos.x -= 0.1f;

	if (Saba::Input::IsKeyPressed(GLFW_KEY_W))		m_CameraPos.y += 0.1f;
	else if (Saba::Input::IsKeyPressed(GLFW_KEY_S)) m_CameraPos.y -= 0.1f;

	if (Saba::Input::IsKeyPressed(GLFW_KEY_E))		m_CameraRotation += 1.0f;
	else if (Saba::Input::IsKeyPressed(GLFW_KEY_Q)) m_CameraRotation -= 1.0f;

	m_Camera.SetPosition(m_CameraPos);
	m_Camera.SetRotation(m_CameraRotation);

	Saba::RenderCommand::Clear();
	Saba::Renderer::BeginScene(m_Camera);

	Saba::Renderer::Submit(m_Shader, m_VAO);

	Saba::Renderer::EndScene();
}
void ExampleLayer::OnImGuiRender()
{
	ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
	bool vsync = Saba::Application::Get()->GetWindow().IsVSync();
	ImGui::Checkbox("Set VSync", &vsync);
	Saba::Application::Get()->GetWindow().SetVSync(vsync);
}
