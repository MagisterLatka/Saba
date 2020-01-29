#include "pch.h"
#include <Saba.h>
#include "ExampleLayer.h"
#include <GLAD\include\glad\glad.h>
#include <imgui/imgui.h>

static uint ShaderDataTypeToOpenGLBaseType(Saba::ShaderDataType type)
{
	switch (type)
	{
		case Saba::ShaderDataType::Float:    return GL_FLOAT;
		case Saba::ShaderDataType::Float2:   return GL_FLOAT;
		case Saba::ShaderDataType::Float3:   return GL_FLOAT;
		case Saba::ShaderDataType::Float4:   return GL_FLOAT;
		case Saba::ShaderDataType::Mat3:     return GL_FLOAT;
		case Saba::ShaderDataType::Mat4:     return GL_FLOAT;
		case Saba::ShaderDataType::Int:      return GL_INT;
		case Saba::ShaderDataType::Int2:     return GL_INT;
		case Saba::ShaderDataType::Int3:     return GL_INT;
		case Saba::ShaderDataType::Int4:     return GL_INT;
		case Saba::ShaderDataType::Bool:     return GL_BOOL;
	}
		
	SB_CORE_ASSERT(false, "Unknown ShaderDataType");
	return 0;
} //Will be moved to OpenGLVertexArray

ExampleLayer::ExampleLayer()
	: Saba::Layer("ExampleLayer")
{
}
ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnAttach()
{
	glCreateVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	////////////////////////////////////////////////////////////////
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	m_VBO.reset(Saba::VertexBuffer::Create(vertices, sizeof(vertices)));

	m_VBO->SetLayout({
		{"i_Pos", Saba::ShaderDataType::Float3}
	});

	uint index = 0;
	const auto& layout = m_VBO->GetLayout();
	for (const auto& element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized, layout.GetStride(), (const void*)element.Offset);
		index++;
	}
	////////////////////////////////////////////////////////////////
	uint indices[] = { 0, 1, 2 };
	m_IBO.reset(Saba::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint)));
	////////////////////////////////////////////////////////////////
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
	glDeleteVertexArrays(1, &m_VAO);
}
void ExampleLayer::OnEvent(Saba::Event& event)
{
}
void ExampleLayer::OnUpdate()
{
	m_Shader->Bind();
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}
void ExampleLayer::OnImGuiRender()
{
	ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
	bool vsync = Saba::Application::Get()->GetWindow().IsVSync();
	ImGui::Checkbox("Set VSync", &vsync);
	Saba::Application::Get()->GetWindow().SetVSync(vsync);
}
