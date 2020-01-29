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
	glCreateVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	////////////////////////////////////////////////////////////////
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	m_VBO.reset(Saba::VertexBuffer::Create(vertices, sizeof(vertices)));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
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

	uint vShader = glCreateShader(GL_VERTEX_SHADER);

	const char* source = vertexSrc.c_str();
	glShaderSource(vShader, 1, &source, nullptr);
	glCompileShader(vShader);

	int result;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int lenght;
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)alloca(lenght);
		glGetShaderInfoLog(vShader, lenght, &lenght, message);
		glDeleteShader(vShader);
		SB_FATAL("Failed to compile vertex shader");
		SB_ASSERT(false, "{0}", message);
	}

	uint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	source = fragmentSrc.c_str();
	glShaderSource(fShader, 1, &source, nullptr);
	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int lenght;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)alloca(lenght);
		glGetShaderInfoLog(fShader, lenght, &lenght, message);
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		SB_FATAL("Failed to compile fragment shader");
		SB_ASSERT(false, "{0}", message);
	}

	m_Shader = glCreateProgram();
	glAttachShader(m_Shader, vShader);
	glAttachShader(m_Shader, fShader);
	glLinkProgram(m_Shader);

	glDetachShader(m_Shader, vShader);
	glDetachShader(m_Shader, fShader);
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	glGetProgramiv(m_Shader, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int lenght;
		glGetProgramiv(m_Shader, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)alloca(lenght);
		glGetProgramInfoLog(m_Shader, lenght, &lenght, message);
		glDeleteProgram(m_Shader);
		SB_FATAL("Failed to link shader");
		SB_ASSERT(false, "{0}", message);
	}
}
void ExampleLayer::OnDetach()
{
	glDeleteProgram(m_Shader);
	glDeleteVertexArrays(1, &m_VAO);
}
void ExampleLayer::OnEvent(Saba::Event& event)
{
}
void ExampleLayer::OnUpdate()
{
	glUseProgram(m_Shader);
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
