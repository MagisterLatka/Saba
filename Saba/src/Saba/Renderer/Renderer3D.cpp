#include "pch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "RenderCommand.h"

namespace Saba {

	constexpr uint32_t c_MaxTriangleCount = 12000;
	constexpr uint32_t c_MaxQuadCount = 9000;
	constexpr uint8_t c_MaxTextures = 32;

	struct VertexData
	{
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec4 color;
		float texID;
	};

	struct Renderer3DData
	{
		Ref<VertexArray> vertexArrayTriangle;
		Ref<VertexArray> vertexArrayQuad;

		uint32_t triangleCount = 0;
		uint32_t quadCount = 0;

		VertexData* bufferTriangle = new VertexData[c_MaxTriangleCount * 3];
		VertexData* atTriangle = bufferTriangle;
		VertexData* bufferQuad = new VertexData[c_MaxTriangleCount * 4];
		VertexData* atQuad = bufferQuad;

		std::array<Ref<Texture2D>, c_MaxTextures> texturesTriangle;
		std::array<Ref<Texture2D>, c_MaxTextures> texturesQuad;
		uint8_t texIndexTriangle = 1;
		uint8_t texIndexQuad = 1;

		Renderer3D::Stats stats;
	};
	static Renderer3DData* s_Data = nullptr;

	void Renderer3D::Init()
	{
		s_Data = new Renderer3DData;

		for (uint32_t i = 0; i < c_MaxQuadCount * 4; i += 4)
		{
			s_Data->bufferQuad[i + 0].uv = { 0.0f, 0.0f };
			s_Data->bufferQuad[i + 1].uv = { 1.0f, 0.0f };
			s_Data->bufferQuad[i + 2].uv = { 1.0f, 1.0f };
			s_Data->bufferQuad[i + 3].uv = { 0.0f, 1.0f };
		}
		
		s_Data->vertexArrayTriangle = VertexArray::Create();
		Ref<VertexBuffer> vbo = VertexBuffer::Create(nullptr, c_MaxTriangleCount * 3 * sizeof(VertexData), Dynamic);
		vbo->SetLayout({
			{"i_Pos", ShaderDataType::Float3},
			{"i_UV", ShaderDataType::Float2},
			{"i_Color", ShaderDataType::Float4},
			{"i_TexID", ShaderDataType::Float}
		});
		s_Data->vertexArrayTriangle->AddVertexBuffer(vbo);

		uint32_t* indices = (uint32_t*)malloc(c_MaxTriangleCount * 3 * sizeof(uint32_t));
		uint32_t offset = 0;
		for (uint32_t i = 0; i < c_MaxTriangleCount * 3u; i += 3)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			offset += 3;
		}
		Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, c_MaxTriangleCount * 3);
		free(indices);
		s_Data->vertexArrayTriangle->SetIndexBuffer(ibo);


		s_Data->vertexArrayQuad = VertexArray::Create();
		vbo = VertexBuffer::Create(nullptr, c_MaxQuadCount * 4 * sizeof(VertexData), Dynamic);
		vbo->SetLayout({
			{"i_Pos", ShaderDataType::Float3},
			{"i_UV", ShaderDataType::Float2},
			{"i_Color", ShaderDataType::Float4},
			{"i_TexID", ShaderDataType::Float}
		});
		s_Data->vertexArrayQuad->AddVertexBuffer(vbo);

		indices = (uint32_t*)malloc(c_MaxQuadCount * 6 * sizeof(uint32_t));
		offset = 0;
		for (uint32_t i = 0; i < c_MaxQuadCount * 6; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}
		ibo = IndexBuffer::Create(indices, c_MaxQuadCount * 6);
		free(indices);
		s_Data->vertexArrayQuad->SetIndexBuffer(ibo);

		Ref<Texture2D> whiteTex = Texture2D::Create(1, 1);
		uint32_t texData = 0xffffffff;
		whiteTex->SetData(&texData, sizeof(uint32_t));
		s_Data->texturesTriangle[0] = whiteTex;
		s_Data->texturesQuad[0] = whiteTex;
	}
	void Renderer3D::Shutdown()
	{
		delete[] s_Data->bufferTriangle;
		delete[] s_Data->bufferQuad;
		delete s_Data;
	}

	void Renderer3D::BeginScene()
	{
		BeginSceneTriangle();
		BeginSceneQuad();
	}
	void Renderer3D::EndScene()
	{
		EndSceneTriangle();
		EndSceneQuad();
	}
	void Renderer3D::Flush()
	{
		FlushTriangle();
		FlushQuad();
	}

	void Renderer3D::BeginSceneTriangle()
	{
		s_Data->atTriangle = s_Data->bufferTriangle;
	}
	void Renderer3D::EndSceneTriangle()
	{
		s_Data->vertexArrayTriangle->GetVertexBuffers()[0]->Bind();
		s_Data->vertexArrayTriangle->GetVertexBuffers()[0]->SetData(s_Data->bufferTriangle, s_Data->triangleCount * 3 * sizeof(VertexData), 0);
		s_Data->texIndexTriangle = 1;
	}
	void Renderer3D::FlushTriangle()
	{
		int index = 0;
		for (Ref<Texture2D> texture : s_Data->texturesTriangle)
		{
			if (texture)
			{
				texture->Bind(index);
				index++;
			}
			else break;
		}

		s_Data->vertexArrayTriangle->Bind();
		RenderCommand::DrawIndexed(s_Data->triangleCount * 6);
		s_Data->triangleCount = 0;
		s_Data->stats.drawCallsOnTriangles++;
	}
	void Renderer3D::BeginSceneQuad()
	{
		s_Data->atQuad = s_Data->bufferQuad;
	}
	void Renderer3D::EndSceneQuad()
	{
		s_Data->vertexArrayQuad->GetVertexBuffers()[0]->Bind();
		s_Data->vertexArrayQuad->GetVertexBuffers()[0]->SetData(s_Data->bufferQuad, s_Data->quadCount * 4 * sizeof(VertexData), 0);
		s_Data->texIndexQuad = 1;
	}
	void Renderer3D::FlushQuad()
	{
		int index = 0;
		for (Ref<Texture2D> texture : s_Data->texturesQuad)
		{
			if (texture)
			{
				texture->Bind(index);
				index++;
			}
			else break;
		}

		s_Data->vertexArrayQuad->Bind();
		RenderCommand::DrawIndexed(s_Data->quadCount * 6);
		s_Data->quadCount = 0;
		s_Data->stats.drawCallsOnQuads++;
	}

	void Renderer3D::DrawTriangle(const std::array<std::pair<glm::vec3, glm::vec2>, 3> & posUV, glm::vec4 color)
	{
		if (s_Data->triangleCount >= c_MaxTriangleCount)
		{
			EndSceneTriangle();
			FlushTriangle();
			BeginSceneTriangle();
		}

		s_Data->atTriangle->pos = posUV[0].first;
		s_Data->atTriangle->uv = posUV[0].second;
		s_Data->atTriangle->color = color;
		s_Data->atTriangle->texID = 0.0f;
		s_Data->atTriangle++;

		s_Data->atTriangle->pos = posUV[1].first;
		s_Data->atTriangle->uv = posUV[1].second;
		s_Data->atTriangle->color = color;
		s_Data->atTriangle->texID = 0.0f;
		s_Data->atTriangle++;

		s_Data->atTriangle->pos = posUV[2].first;
		s_Data->atTriangle->uv = posUV[2].second;
		s_Data->atTriangle->color = color;
		s_Data->atTriangle->texID = 0.0f;
		s_Data->atTriangle++;

		s_Data->triangleCount++;
		s_Data->stats.triangleCount++;
	}
	void Renderer3D::DrawTriangle(const std::array<std::pair<glm::vec3, glm::vec2>, 3> & posUV, Ref<Texture2D> texture)
	{
		if (s_Data->triangleCount >= c_MaxTriangleCount || s_Data->texIndexTriangle > c_MaxTextures - 1)
		{
			EndSceneTriangle();
			FlushTriangle();
			BeginSceneTriangle();
		}

		float tid = 0.0f;
		for (uint8_t i = 1; i < s_Data->texIndexTriangle; i++)
		{
			if (s_Data->texturesTriangle[i] == texture)
			{
				tid = (float)i;
				break;
			}
		}
		if (tid == 0.0f)
		{
			tid = (float)s_Data->texIndexTriangle;
			s_Data->texturesTriangle[s_Data->texIndexTriangle] = texture;
			s_Data->texIndexTriangle++;
		}

		s_Data->atTriangle->pos = posUV[0].first;
		s_Data->atTriangle->uv = posUV[0].second;
		s_Data->atTriangle->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s_Data->atTriangle->texID = tid;
		s_Data->atTriangle++;

		s_Data->atTriangle->pos = posUV[1].first;
		s_Data->atTriangle->uv = posUV[1].second;
		s_Data->atTriangle->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s_Data->atTriangle->texID = tid;
		s_Data->atTriangle++;

		s_Data->atTriangle->pos = posUV[2].first;
		s_Data->atTriangle->uv = posUV[2].second;
		s_Data->atTriangle->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s_Data->atTriangle->texID = tid;
		s_Data->atTriangle++;

		s_Data->triangleCount++;
		s_Data->stats.triangleCount++;
	}

	void Renderer3D::DrawQuad(const std::array<glm::vec3, 4> & pos, glm::vec4 color)
	{
		if (s_Data->triangleCount >= c_MaxTriangleCount)
		{
			EndSceneQuad();
			FlushQuad();
			BeginSceneQuad();
		}

		s_Data->atQuad->pos = pos[0];
		s_Data->atQuad->color = color;
		s_Data->atQuad->texID = 0.0f;
		s_Data->atQuad++;

		s_Data->atQuad->pos = pos[1];
		s_Data->atQuad->color = color;
		s_Data->atQuad->texID = 0.0f;
		s_Data->atQuad++;

		s_Data->atQuad->pos = pos[2];
		s_Data->atQuad->color = color;
		s_Data->atQuad->texID = 0.0f;
		s_Data->atQuad++;

		s_Data->atQuad->pos = pos[3];
		s_Data->atQuad->color = color;
		s_Data->atQuad->texID = 0.0f;
		s_Data->atQuad++;

		s_Data->quadCount++;
		s_Data->stats.quadCount++;
	}
	void Renderer3D::DrawQuad(const std::array<glm::vec3, 4> & pos, Ref<Texture2D> texture)
	{
		if (s_Data->quadCount >= c_MaxQuadCount || s_Data->texIndexQuad > c_MaxTextures - 1)
		{
			EndSceneQuad();
			FlushQuad();
			BeginSceneQuad();
		}

		float tid = 0.0f;
		for (uint8_t i = 1; i < s_Data->texIndexQuad; i++)
		{
			if (s_Data->texturesQuad[i] == texture)
			{
				tid = (float)i;
				break;
			}
		}
		if (tid == 0.0f)
		{
			tid = (float)s_Data->texIndexQuad;
			s_Data->texturesQuad[s_Data->texIndexQuad] = texture;
			s_Data->texIndexQuad++;
		}

		s_Data->atQuad->pos = pos[0];
		s_Data->atQuad->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s_Data->atQuad->texID = tid;
		s_Data->atQuad++;

		s_Data->atQuad->pos = pos[1];
		s_Data->atQuad->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s_Data->atQuad->texID = tid;
		s_Data->atQuad++;

		s_Data->atQuad->pos = pos[2];
		s_Data->atQuad->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s_Data->atQuad->texID = tid;
		s_Data->atQuad++;

		s_Data->atQuad->pos = pos[3];
		s_Data->atQuad->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s_Data->atQuad->texID = tid;
		s_Data->atQuad++;

		s_Data->quadCount++;
		s_Data->stats.quadCount++;
	}

	void Renderer3D::ResetStats()
	{
		s_Data->stats.triangleCount = 0;
		s_Data->stats.drawCallsOnTriangles = 0;
		s_Data->stats.quadCount = 0;
		s_Data->stats.drawCallsOnQuads = 0;
	}
	const Renderer3D::Stats& Renderer3D::GetStats()
	{
		return s_Data->stats;
	}

}
