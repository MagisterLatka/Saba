#include "pch.h"
#include <Saba.h>
#include "ExampleLayer.h"
#include <GLAD\include\glad\glad.h>
#include <imgui/imgui.h>

ExampleLayer::ExampleLayer()
	: Saba::Layer("ExampleLayer")
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

		void main()
		{
			gl_Position = vec4(i_Pos, 1.0f);
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
}
void ExampleLayer::OnDetach()
{
}
void ExampleLayer::OnEvent(Saba::Event& event)
{
}
void ExampleLayer::OnUpdate()
{
	m_Shader->Bind();
	Saba::Renderer::Submit(m_VAO);
}
void ExampleLayer::OnImGuiRender()
{
	ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
	bool vsync = Saba::Application::Get()->GetWindow().IsVSync();
	ImGui::Checkbox("Set VSync", &vsync);
	Saba::Application::Get()->GetWindow().SetVSync(vsync);
}
