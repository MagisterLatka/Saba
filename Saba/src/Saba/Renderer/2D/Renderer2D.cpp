#include "pch.h"
#include "Renderer2D.h"

#include "Saba\Renderer\VertexArray.h"
#include "Saba\Renderer\Renderer.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Saba {

	constexpr uint32_t c_MaxQuadCount = 10000;
	constexpr uint8_t c_MaxTextures = 32;

	struct VertexData
	{
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec4 color;
		glm::vec2 texID_TillingFactor;
	};

	struct Renderer2DData
	{
		Ref<VertexArray> vertexArray;
		uint32_t quadCount = 0;

		VertexData* buffer = new VertexData[c_MaxQuadCount * 4];
		VertexData* at = buffer;

		std::array<Ref<Texture2D>, c_MaxTextures> textures;
		uint8_t texIndex = 1;

		Renderer2D::Stats stats;
	};
	static Renderer2DData* s_RendererData = nullptr;

	void Renderer2D::Init()
	{
		s_RendererData = new Renderer2DData;

		for (uint32_t i = 0; i < c_MaxQuadCount * 4; i += 4)
		{
			s_RendererData->buffer[i + 0].uv = { 0.0f, 0.0f };
			s_RendererData->buffer[i + 1].uv = { 1.0f, 0.0f };
			s_RendererData->buffer[i + 2].uv = { 1.0f, 1.0f };
			s_RendererData->buffer[i + 3].uv = { 0.0f, 1.0f };
		}

		s_RendererData->vertexArray = VertexArray::Create();
		Ref<VertexBuffer> vbo = VertexBuffer::Create(nullptr, c_MaxQuadCount * 4 * sizeof(VertexData), Dynamic);
		vbo->SetLayout({
			{"i_Pos", ShaderDataType::Float3},
			{"i_UV", ShaderDataType::Float2},
			{"i_Color", ShaderDataType::Float4},
			{"i_TexID_TillingFactor", ShaderDataType::Float2}
		});
		s_RendererData->vertexArray->AddVertexBuffer(vbo);

		uint32_t* indices = (uint32_t*)malloc(c_MaxQuadCount * 6 * sizeof(uint32_t));
		uint32_t offset = 0;
		for (int i = 0; i < c_MaxQuadCount * 6; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, c_MaxQuadCount * 6);
		free(indices);
		s_RendererData->vertexArray->SetIndexBuffer(ibo);

		s_RendererData->textures[0] = Texture2D::Create(1, 1);
		uint32_t texData = 0xffffffff;
		s_RendererData->textures[0]->SetData(&texData, sizeof(uint32_t));
	}
	void Renderer2D::Shutdown()
	{
		delete[] s_RendererData->buffer;
		delete s_RendererData;
	}

	void Renderer2D::Flush()
	{
		if (s_RendererData->quadCount > 0)
		{
			s_RendererData->vertexArray->GetVertexBuffers()[0]->Bind();
			s_RendererData->vertexArray->GetVertexBuffers()[0]->SetData(s_RendererData->buffer, s_RendererData->quadCount * 4 * sizeof(VertexData), 0);
			s_RendererData->texIndex = 1;

			int index = 0;
			for (Ref<Texture2D> texture : s_RendererData->textures)
			{
				if (texture)
				{
					texture->Bind(index);
					index++;
				}
				else break;
			}

			s_RendererData->vertexArray->Bind();
			RenderCommand::DrawIndexed(s_RendererData->quadCount * 6);

			s_RendererData->quadCount = 0;
			s_RendererData->at = s_RendererData->buffer;
			for (int i = 1; i < c_MaxTextures; i++)
				s_RendererData->textures[i].reset();
			s_RendererData->stats.drawCalls++;
		}
	}

	void Renderer2D::DrawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color, Ref<Texture2D> texture, float tillingFactor)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, size, color, texture, tillingFactor);
	}
	void Renderer2D::DrawQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color, Ref<Texture2D> texture, float tillingFactor)
	{
		if (s_RendererData->quadCount >= c_MaxQuadCount || s_RendererData->texIndex > c_MaxTextures - 1)
			Flush();

		float tid = 0.0f;
		if (texture)
		{
			for (uint8_t i = 1; i < s_RendererData->texIndex; i++)
			{
				if (s_RendererData->textures[i] == texture)
				{
					tid = (float)i;
					break;
				}
			}
			if (tid == 0.0f)
			{
				tid = (float)s_RendererData->texIndex;
				s_RendererData->textures[s_RendererData->texIndex] = texture;
				s_RendererData->texIndex++;
			}
		}

		static constexpr glm::vec3 data[4] = {
			{ -0.5f, -0.5f, 0.0f },
			{  0.5f, -0.5f, 0.0f },
			{  0.5f,  0.5f, 0.0f },
			{ -0.5f,  0.5f, 0.0f }
		};

		for (int i = 0; i < 4; i++)
		{
			s_RendererData->at->pos = data[i] * glm::vec3(size, 1.0f) + pos;
			s_RendererData->at->color = color;
			s_RendererData->at->texID_TillingFactor = { tid, tillingFactor };
			s_RendererData->at++;
		}

		s_RendererData->quadCount++;
		s_RendererData->stats.quadCount++;
	}

	void Renderer2D::DrawRotatedQuad(glm::vec2 pos, glm::vec2 size, float angleD, glm::vec4 color, Ref<Texture2D> texture, float tillingFactor)
	{
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, size, angleD, color, texture, tillingFactor);
	}
	void Renderer2D::DrawRotatedQuad(glm::vec3 pos, glm::vec2 size, float angleD, glm::vec4 color, Ref<Texture2D> texture, float tillingFactor)
	{
		if (s_RendererData->quadCount >= c_MaxQuadCount || s_RendererData->texIndex > c_MaxTextures - 1)
			Flush();

		float tid = 0.0f;
		if (texture)
		{
			for (uint8_t i = 1; i < s_RendererData->texIndex; i++)
			{
				if (s_RendererData->textures[i] == texture)
				{
					tid = (float)i;
					break;
				}
			}
			if (tid == 0.0f)
			{
				tid = (float)s_RendererData->texIndex;
				s_RendererData->textures[s_RendererData->texIndex] = texture;
				s_RendererData->texIndex++;
			}
		}

		static constexpr glm::vec3 data[4] = {
			{ -0.5f, -0.5f, 0.0f },
			{  0.5f, -0.5f, 0.0f },
			{  0.5f,  0.5f, 0.0f },
			{ -0.5f,  0.5f, 0.0f }
		};
		glm::mat3 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angleD), { 0.0f, 0.0f, 1.0f });

		for (int i = 0; i < 4; i++)
		{
			s_RendererData->at->pos = (rotate * data[i]) * glm::vec3(size, 1.0f) + pos;
			s_RendererData->at->color = color;
			s_RendererData->at->texID_TillingFactor = { tid, tillingFactor };
			s_RendererData->at++;
		}

		s_RendererData->quadCount++;
		s_RendererData->stats.quadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_RendererData->stats, 0, sizeof(Renderer2D::Stats));
	}

	const Renderer2D::Stats& Renderer2D::GetStats()
	{
		return s_RendererData->stats;
	}
}