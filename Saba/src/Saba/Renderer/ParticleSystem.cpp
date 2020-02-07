#include "pch.h"
#include "ParticleSystem.h"
#include "Saba\Random.h"
#include "Renderer.h"

#include <glm\gtc\constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\compatibility.hpp>

namespace Saba {

	ParticleSystem::ParticleSystem()
	{
		m_Particles.resize(10000);
	}

	void ParticleSystem::OnUpdate(Timestep ts)
	{
		for (Particle& particle : m_Particles)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeCounter <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeCounter -= (float)ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += 0.01f * (float)ts;
		}
	}
	void ParticleSystem::OnRender()
	{
		if (!m_VAO)
		{
			m_VAO = VertexArray::Create();
			m_VAO->Bind();

			float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f
			};
			Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));
			vbo->SetLayout({
				{"position", ShaderDataType::Float3}
			});
			m_VAO->AddVertexBuffer(vbo);

			uint indices[] = {
				0, 1, 2, 2, 3, 0
			};
			Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, 6);
			m_VAO->SetIndexBuffer(ibo);

			std::string vertexSrc = R"(
				#version 330 core

				layout(location = 0) in vec3 i_Pos;

				uniform mat4 u_ViewProjMat;
				uniform mat4 u_Transform;

				void main()
				{
					gl_Position = u_ViewProjMat * u_Transform * vec4(i_Pos, 1.0f);
				}
			)";

			std::string fragmentSrc = R"(
				#version 330 core

				out vec4 o_Color;

				uniform vec4 u_Color;

				void main()
				{
					o_Color = u_Color;
				}
			)";
			m_Shader = Shader::Create(vertexSrc.c_str(), fragmentSrc.c_str());
		}

		for (auto& particle : m_Particles)
		{
			if (!particle.Active)
				continue;

			float life = particle.LifeCounter / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			color.a *= life;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), {particle.Position.x, particle.Position.y, 0.0f}),
											 particle.Rotation, { 0.0f, 0.0f, 1.0f }), glm::vec3(size, size, 0.0f));

			m_Shader->SetUniformMat4("u_Transform", transform);
			m_Shader->SetUniformFloat4("u_Color", color);
			Renderer::Submit(m_Shader, m_VAO);
		}
	}

	void ParticleSystem::Emit(ParticleProps& props)
	{
		Particle& particle = m_Particles[m_Index];
		particle.Active = true;
		particle.Position = props.Position;
		particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

		particle.Velocity = props.Velocity;
		particle.Velocity.x += props.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += props.VelocityVariation.y * (Random::Float() - 0.5f);

		particle.ColorBegin = props.ColorBegin;
		particle.ColorEnd = props.ColorEnd;

		particle.LifeTime = props.LifeTime;
		particle.LifeCounter = props.LifeTime;
		particle.SizeBegin = props.SizeBegin + props.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = props.SizeEnd;

		m_Index = --m_Index % m_Particles.size();
	}
}
