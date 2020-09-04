#include "pch.h"
#include "Saba/Renderer/Renderer2D.h"

#include "Saba/Renderer/VertexArray.h"
#include "Saba/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Saba {

	static constexpr uint32_t c_MaxQuadCount = 20000;
	static constexpr uint8_t c_MaxTextures = 32;

	struct VertexData
	{
		glm::vec3 pos;
		glm::vec4 uv_texID_TillingFactor;
		glm::vec4 color;
	};

	struct Renderer2DData
	{
		Ref<Shader> defaultShader;
		Ref<Shader> activeShader;

		Ref<VertexArray> vertexArray;
		uint32_t quadCount = 0;

		VertexData* buffer = new VertexData[c_MaxQuadCount * 4];
		VertexData* at = buffer;

		std::array<Ref<Texture2D>, c_MaxTextures> textures;
		uint8_t texIndex = 1;

		Renderer2D::Stats stats;

		static constexpr glm::vec3 quadData[4] = {
			{ -0.5f, -0.5f, 0.0f },
			{  0.5f, -0.5f, 0.0f },
			{  0.5f,  0.5f, 0.0f },
			{ -0.5f,  0.5f, 0.0f }
		};
	};
	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 i_Pos;
			layout(location = 1) in vec4 i_UV_TID_TillingFactor;
			layout(location = 2) in vec4 i_Color;

			out DATA {
				vec2 uv;
				vec4 color;
				float tid;
				float tillingFactor;
			} vs_out;

			uniform mat4 u_ViewProjMat;

			void main()
			{
				gl_Position = u_ViewProjMat * vec4(i_Pos, 1.0f);

				vs_out.uv = i_UV_TID_TillingFactor.xy;
				vs_out.color = i_Color;
				vs_out.tid = i_UV_TID_TillingFactor.z;
				vs_out.tillingFactor = i_UV_TID_TillingFactor.w;
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core

			out vec4 o_Color;

			in DATA {
				vec2 uv;
				vec4 color;
				float tid;
				float tillingFactor;
			} fs_in;

			uniform sampler2D u_Textures[32];

			void main()
			{
				o_Color = texture(u_Textures[int(fs_in.tid)], fs_in.uv * fs_in.tillingFactor) * fs_in.color;
			}
		)";
		s_Data.defaultShader = Shader::Create(vertexSrc, fragmentSrc);
		s_Data.defaultShader->Bind();
		static int texIDs[] = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
		};
		s_Data.defaultShader->SetUniformInt1v("u_Textures", texIDs, 32);
		s_Data.activeShader = s_Data.defaultShader;

		for (uint32_t i = 0; i < c_MaxQuadCount * 4; i += 4)
		{
			s_Data.buffer[i + 0].uv_texID_TillingFactor = { 0.0f, 0.0f, 0.0f, 1.0f };
			s_Data.buffer[i + 1].uv_texID_TillingFactor = { 1.0f, 0.0f, 0.0f, 1.0f };
			s_Data.buffer[i + 2].uv_texID_TillingFactor = { 1.0f, 1.0f, 0.0f, 1.0f };
			s_Data.buffer[i + 3].uv_texID_TillingFactor = { 0.0f, 1.0f, 0.0f, 1.0f };
		}

		s_Data.vertexArray = VertexArray::Create();
		Ref<VertexBuffer> vbo = VertexBuffer::Create(nullptr, c_MaxQuadCount * 4 * sizeof(VertexData), BufferUsage::Dynamic);
		vbo->SetLayout({
			{"i_Pos", ShaderDataType::Float3},
			{"i_UV_TexID_TillingFactor", ShaderDataType::Float4},
			{"i_Color", ShaderDataType::Float4}
		});
		s_Data.vertexArray->AddVertexBuffer(vbo);

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
		s_Data.vertexArray->SetIndexBuffer(ibo);

		s_Data.textures[0] = Texture2D::Create(1, 1);
		uint32_t texData = 0xffffffff;
		s_Data.textures[0]->SetData(&texData, sizeof(uint32_t));
	}
	void Renderer2D::Shutdown()
	{
		delete[] s_Data.buffer;
		s_Data.vertexArray.reset();
	}

	void Renderer2D::SetShader(Ref<Shader> shader)
	{
		if (shader)
			s_Data.activeShader = shader;
		else
			s_Data.activeShader = s_Data.defaultShader;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& tranform)
	{
		s_Data.activeShader->Bind();
		s_Data.activeShader->SetUniformMat4("u_ViewProjMat", camera.GetProjection() * glm::inverse(tranform));
	}

	void Renderer2D::BeginScene(const glm::mat4& viewProjectionMatrix)
	{
		s_Data.activeShader->Bind();
		s_Data.activeShader->SetUniformMat4("u_ViewProjMat", viewProjectionMatrix);
	}

	void Renderer2D::Flush()
	{
		if (s_Data.quadCount > 0)
		{
			s_Data.vertexArray->GetVertexBuffers()[0]->SetData(s_Data.buffer, s_Data.quadCount * 4 * sizeof(VertexData), 0);
			s_Data.texIndex = 1;

			int index = 0;
			for (Ref<Texture2D> texture : s_Data.textures)
			{
				if (texture)
				{
					texture->Bind(index);
					index++;
				}
				else break;
			}

			RenderCommand::DrawIndexed(s_Data.vertexArray, s_Data.quadCount * 6);

			s_Data.quadCount = 0;
			s_Data.at = s_Data.buffer;
			for (int i = 1; i < c_MaxTextures; i++)
				s_Data.textures[i].reset();
			s_Data.stats.drawCalls++;
		}
	}

	void Renderer2D::DrawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color, const Ref<Texture2D>& texture, float tillingFactor)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, size, color, texture, tillingFactor);
	}
	void Renderer2D::DrawQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color, const Ref<Texture2D>& texture, float tillingFactor)
	{
		if (s_Data.quadCount >= c_MaxQuadCount || s_Data.texIndex > c_MaxTextures - 1)
			Flush();

		float tid = 0.0f;
		if (texture)
		{
			for (uint8_t i = 1; i < s_Data.texIndex; i++)
			{
				if (s_Data.textures[i] == texture)
				{
					tid = (float)i;
					break;
				}
			}
			if (tid == 0.0f)
			{
				tid = (float)s_Data.texIndex;
				s_Data.textures[s_Data.texIndex] = texture;
				s_Data.texIndex++;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			s_Data.at->pos = s_Data.quadData[i] * glm::vec3(size, 1.0f) + pos;
			s_Data.at->uv_texID_TillingFactor.z = tid;
			s_Data.at->uv_texID_TillingFactor.w = tillingFactor;
			s_Data.at->color = color;
			s_Data.at++;
		}

		s_Data.quadCount++;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::DrawQuad(glm::mat4 transform, glm::vec4 color, const Ref<Texture2D>& texture, float tillingFactor)
	{
		if (s_Data.quadCount >= c_MaxQuadCount || s_Data.texIndex > c_MaxTextures - 1)
			Flush();

		float tid = 0.0f;
		if (texture)
		{
			for (uint8_t i = 1; i < s_Data.texIndex; i++)
			{
				if (s_Data.textures[i] == texture)
				{
					tid = (float)i;
					break;
				}
			}
			if (tid == 0.0f)
			{
				tid = (float)s_Data.texIndex;
				s_Data.textures[s_Data.texIndex] = texture;
				s_Data.texIndex++;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			s_Data.at->pos = transform * glm::vec4(s_Data.quadData[i], 1.0f);
			s_Data.at->color = color;
			s_Data.at->uv_texID_TillingFactor.z = tid;
			s_Data.at->uv_texID_TillingFactor.w = tillingFactor;
			s_Data.at++;
		}

		s_Data.quadCount++;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::DrawRotatedQuad(glm::vec2 pos, glm::vec2 size, float angle, glm::vec4 color, const Ref<Texture2D>& texture, float tillingFactor)
	{
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, size, angle, color, texture, tillingFactor);
	}
	void Renderer2D::DrawRotatedQuad(glm::vec3 pos, glm::vec2 size, float angle, glm::vec4 color, const Ref<Texture2D>& texture, float tillingFactor)
	{
		if (s_Data.quadCount >= c_MaxQuadCount || s_Data.texIndex > c_MaxTextures - 1)
			Flush();

		float tid = 0.0f;
		if (texture)
		{
			for (uint8_t i = 1; i < s_Data.texIndex; i++)
			{
				if (s_Data.textures[i] == texture)
				{
					tid = (float)i;
					break;
				}
			}
			if (tid == 0.0f)
			{
				tid = (float)s_Data.texIndex;
				s_Data.textures[s_Data.texIndex] = texture;
				s_Data.texIndex++;
			}
		}
		glm::mat3 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angle), { 0.0f, 0.0f, 1.0f });

		for (int i = 0; i < 4; i++)
		{
			s_Data.at->pos = (rotate * s_Data.quadData[i]) * glm::vec3(size, 1.0f) + pos;
			s_Data.at->color = color;
			s_Data.at->uv_texID_TillingFactor.z = tid;
			s_Data.at->uv_texID_TillingFactor.w = tillingFactor;
			s_Data.at++;
		}

		s_Data.quadCount++;
		s_Data.stats.quadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.stats, 0, sizeof(Renderer2D::Stats));
	}

	const Renderer2D::Stats& Renderer2D::GetStats()
	{
		return s_Data.stats;
	}
}
