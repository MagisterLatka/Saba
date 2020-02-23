#include "pch.h"
#include "ParticleSystemBatched.h"

#include <glm\gtc\constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\compatibility.hpp>

#include "Renderer.h"

namespace Saba {

	struct VertexData
	{
		glm::vec3 pos;
		glm::vec4 color;
	};

	ParticleSystemBatched::ParticleSystemBatched()
		: ParticleSystem(), m_Buffer((float*)calloc(m_Particles.size() * 4, sizeof(VertexData)))
	{
	}

	ParticleSystemBatched::~ParticleSystemBatched()
	{
		delete[] m_Buffer;
	}

	void ParticleSystemBatched::OnRender()
	{
		if (!m_VAO)
		{
			m_VAO = VertexArray::Create();
			m_VAO->Bind();

			Ref<VertexBuffer> vbo = VertexBuffer::Create(nullptr, sizeof(VertexData) * 4 * m_Particles.size(), Stream);
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
			Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, m_Particles.size() * 6);
			m_VAO->SetIndexBuffer(ibo);

			m_Shader = Shader::Create("assets/shaders/particleBatched.glsl");
		}

		VertexData* at = (VertexData*)m_Buffer;
		for (auto& particle : m_Particles)
		{
			if (!particle.Active)
			{
				at->color.a = 0.0f;
				at++;
				at->color.a = 0.0f;
				at++;
				at->color.a = 0.0f;
				at++;
				at->color.a = 0.0f;
				at++;
				continue;
			}

			static glm::vec4 data[4] = {
				{-0.5f, -0.5f, 0.0f, 1.0f},
				{ 0.5f, -0.5f, 0.0f, 1.0f},
				{ 0.5f,  0.5f, 0.0f, 1.0f},
				{-0.5f,  0.5f, 0.0f, 1.0f}
			};

			float life = particle.LifeCounter / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			color.a *= life;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), {particle.Position.x, particle.Position.y, 0.0f}),
											 particle.Rotation, { 0.0f, 0.0f, 1.0f }), glm::vec3(size, size, 0.0f));

			at->pos = glm::vec3(transform * data[0]);
			at->color = color;
			at++;
			at->pos = glm::vec3(transform * data[1]);
			at->color = color;
			at++;
			at->pos = glm::vec3(transform * data[2]);
			at->color = color;
			at++;
			at->pos = glm::vec3(transform * data[3]);
			at->color = color;
			at++;
		}
		m_VAO->GetVertexBuffers()[0]->Bind();
		m_VAO->GetVertexBuffers()[0]->SetSubdata(0, sizeof(VertexData) * 4 * m_Particles.size(), m_Buffer);
		
		Renderer::Submit(m_Shader, m_VAO);
	}

}
