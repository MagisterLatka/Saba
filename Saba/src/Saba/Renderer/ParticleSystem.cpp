#include "pch.h"
#include "Saba/Renderer/ParticleSystem.h"
#include "Saba/Random.h"
#include "Saba/Renderer/RenderCommand.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace Saba {

	struct VertexData
	{
		glm::vec3 pos;
		glm::vec4 color;
	};

	ParticleSystem::ParticleSystem(uint32_t maxParticles)
		: m_Index(maxParticles - 1)
	{
		m_Particles.resize(maxParticles);
		m_Buffer = (float*)malloc(m_Particles.size() * 4 * sizeof(VertexData));
	}
	ParticleSystem::~ParticleSystem()
	{
		free(m_Buffer);
	}

	void ParticleSystem::OnUpdate(Timestep ts)
	{
		m_FirstActive = -1; m_LastActive = -1;
		uint32_t current = 0;
		for (Particle& particle : m_Particles)
		{
			if (!particle.Active)
			{
				current++;
				continue;
			}

			if (m_FirstActive == -1)
				m_FirstActive = current;
			m_LastActive = current;

			current++;

			if (particle.LifeCounter <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeCounter -= (float)ts;
			particle.Position += glm::vec3(particle.Velocity, -0.01f / particle.LifeTime) * (float)ts;
			particle.Rotation += 0.01f * (float)ts;
		}
	}
	void ParticleSystem::OnRender()
	{
		if (!m_VAO)
		{
			m_VAO = VertexArray::Create();
			m_VAO->Bind();

			Ref<VertexBuffer> vbo = VertexBuffer::Create(nullptr, (uint32_t)m_Particles.size() * 4 * sizeof(VertexData), BufferUsage::Dynamic);
			vbo->SetLayout({
				{"position", ShaderDataType::Float3},
				{"color", ShaderDataType::Float4}
			});
			m_VAO->AddVertexBuffer(vbo);

			uint32_t* indices = new uint32_t[m_Particles.size() * 6];
			uint32_t index = 0;
			for (int i = 0; i < m_Particles.size() * 6; i += 6)
			{
				indices[i + 0] = index + 0;
				indices[i + 1] = index + 1;
				indices[i + 2] = index + 2;

				indices[i + 3] = index + 2;
				indices[i + 4] = index + 3;
				indices[i + 5] = index + 0;

				index += 4;
			}
			Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, (uint32_t)m_Particles.size() * 6);
			delete[] indices;
			m_VAO->SetIndexBuffer(ibo);
		}

		if (m_FirstActive != -1)
		{
			VertexData* at = (VertexData*)m_Buffer;
			at += (long long)m_FirstActive * 4;
			for (uint32_t i = m_FirstActive; i <= m_LastActive; i++)
			{
				auto& particle = m_Particles[i];
				if (!particle.Active)
				{
					for (int j = 0; j < 4; j++)
					{
						at->color.a = 0.0f;
						at++;
					}
					continue;
				}

				static constexpr glm::vec3 data[4] = {
					{-0.5f, -0.5f, 0.0f },
					{ 0.5f, -0.5f, 0.0f },
					{ 0.5f,  0.5f, 0.0f },
					{-0.5f,  0.5f, 0.0f }
				};

				float life = particle.LifeCounter / particle.LifeTime;
				glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
				color.a *= life;

				float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

				glm::mat3 rotate = glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f });

				for (int j = 0; j < 4; j++)
				{
					at->pos = (rotate * data[j]) * size + particle.Position;
					at->color = color;
					at++;
				}
			}
			m_VAO->GetVertexBuffers()[0]->SetData(m_Buffer + m_FirstActive * 28ULL, (m_LastActive - m_FirstActive + 1) * 4 * sizeof(VertexData), m_FirstActive * 4 * sizeof(VertexData));
			RenderCommand::DrawIndexed(m_VAO);
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
