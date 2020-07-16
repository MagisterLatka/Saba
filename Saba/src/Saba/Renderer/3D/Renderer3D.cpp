#include "pch.h"
#include "Renderer3D.h"

#include "Saba\Renderer\RenderCommand.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Saba {

	Renderer3D::Renderer3DData Renderer3D::s_Data;

	void Renderer3D::Init()
	{
		for (uint32_t i = 0; i < c_MaxQuadCount * 4; i += 4)
		{
			s_Data.bufferQuad[i + 0].uv = { 0.0f, 0.0f };
			s_Data.bufferQuad[i + 1].uv = { 1.0f, 0.0f };
			s_Data.bufferQuad[i + 2].uv = { 1.0f, 1.0f };
			s_Data.bufferQuad[i + 3].uv = { 0.0f, 1.0f };
		}
		
		s_Data.vertexArrayTriangle = VertexArray::Create();
		Ref<VertexBuffer> vbo = VertexBuffer::Create(nullptr, c_MaxTriangleCount * 3 * sizeof(VertexData), Dynamic);
		vbo->SetLayout({
			{"i_Pos_IsLighted", ShaderDataType::Float4},
			{"i_Normal", ShaderDataType::Float3 },
			{"i_UV", ShaderDataType::Float2},
			{"i_Color", ShaderDataType::Float4},
			{"i_TexID", ShaderDataType::Float}
		});
		s_Data.vertexArrayTriangle->AddVertexBuffer(vbo);

		ModelVertexBuffer dataModel = {
			glm::mat4(1.0f),
			glm::vec4(1.0f),
			glm::vec2(0.0f, 1.0f)
		};
		vbo = VertexBuffer::Create((float*)&dataModel, sizeof(ModelVertexBuffer));
		vbo->SetLayout({
			{ "i_ModelMat", ShaderDataType::Mat4, 1 },
			{ "i_ColorMul", ShaderDataType::Float4, 1 },
			{ "i_TIDoptional_IsLighted", ShaderDataType::Float2, 1 }
		});
		s_Data.vertexArrayTriangle->AddVertexBuffer(vbo);

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
		s_Data.vertexArrayTriangle->SetIndexBuffer(ibo);


		s_Data.vertexArrayQuad = VertexArray::Create();
		Ref<VertexBuffer> vbo2 = VertexBuffer::Create(nullptr, c_MaxQuadCount * 4 * sizeof(VertexData), Dynamic);
		vbo2->SetLayout({
			{"i_Pos_IsLighted", ShaderDataType::Float4},
			{"i_Normal", ShaderDataType::Float3 },
			{"i_UV", ShaderDataType::Float2},
			{"i_Color", ShaderDataType::Float4},
			{"i_TexID", ShaderDataType::Float}
		});
		s_Data.vertexArrayQuad->AddVertexBuffer(vbo2);
		s_Data.vertexArrayQuad->AddVertexBuffer(vbo);

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
		s_Data.vertexArrayQuad->SetIndexBuffer(ibo);


		Ref<Texture2D> whiteTex = Texture2D::Create(1, 1, Saba::Texture::Format::RGBA8);
		uint32_t texData = 0xffffffff;
		whiteTex->SetData(&texData, sizeof(uint32_t));
		s_Data.texturesTriangle[0] = whiteTex;
		s_Data.texturesQuad[0] = whiteTex;
		s_Data.whiteTex = whiteTex;
	}
	void Renderer3D::Shutdown()
	{
		delete[] s_Data.bufferTriangle;
		delete[] s_Data.bufferQuad;
		for (int i = 0; i < s_Data.modelData.size(); i++)
			delete[] s_Data.modelData[i].modelMatBuffer;
	}

	void Renderer3D::Flush()
	{
		FlushTriangle();
		FlushQuad();
		for (int i = 0; i < s_Data.modelData.size(); i++)
			FlushModel(i);
	}

	void Renderer3D::FlushTriangle()
	{
		if (s_Data.triangleCount != 0)
		{
			s_Data.vertexArrayTriangle->GetVertexBuffers()[0]->Bind();
			s_Data.vertexArrayTriangle->GetVertexBuffers()[0]->SetData(s_Data.bufferTriangle, s_Data.triangleCount * 3 * sizeof(VertexData), 0);
			s_Data.texIndexTriangle = 1;

			int index = 0;
			for (Ref<Texture2D> texture : s_Data.texturesTriangle)
			{
				if (texture)
				{
					texture->Bind(index);
					index++;
				}
				else break;
			}

			s_Data.vertexArrayTriangle->Bind();
			RenderCommand::DrawIndexedInstanced(s_Data.triangleCount * 6, 1);
			s_Data.triangleCount = 0;
			s_Data.stats.drawCallsOnTriangles++;

			s_Data.atTriangle = s_Data.bufferTriangle;
		}
	}

	void Renderer3D::FlushQuad()
	{
		if (s_Data.quadCount != 0)
		{
			s_Data.vertexArrayQuad->GetVertexBuffers()[0]->Bind();
			s_Data.vertexArrayQuad->GetVertexBuffers()[0]->SetData(s_Data.bufferQuad, s_Data.quadCount * 4 * sizeof(VertexData), 0);
			s_Data.texIndexQuad = 1;

			int index = 0;
			for (Ref<Texture2D> texture : s_Data.texturesQuad)
			{
				if (texture)
				{
					texture->Bind(index);
					index++;
				}
				else break;
			}

			s_Data.vertexArrayQuad->Bind();
			RenderCommand::DrawIndexedInstanced(s_Data.quadCount * 6, 1);
			s_Data.quadCount = 0;
			s_Data.stats.drawCallsOnQuads++;
			
			s_Data.atQuad = s_Data.bufferQuad;
		}
	}

	void Renderer3D::FlushModel(uint8_t modelID)
	{
		if (s_Data.modelData[modelID].instancesCount != 0)
		{
			s_Data.modelData[modelID].vertexArray->GetVertexBuffers()[1]->Bind();
			s_Data.modelData[modelID].vertexArray->GetVertexBuffers()[1]->SetData(s_Data.modelData[modelID].modelMatBuffer, s_Data.modelData[modelID].instancesCount * sizeof(ModelData), 0);

			int index = 0;
			for (auto& texture : s_Data.modelData[modelID].textures)
			{
				if (texture)
				{
					texture->Bind(index);
					index++;
				}
				else break;
			}

			s_Data.modelData[modelID].vertexArray->Bind();
			RenderCommand::DrawIndexedInstanced(s_Data.modelData[modelID].vertexArray, s_Data.modelData[modelID].instancesCount, s_Data.modelData[modelID].topology);
			s_Data.modelData[modelID].instancesCount = 0;

			s_Data.modelData[modelID].at = s_Data.modelData[modelID].modelMatBuffer;
		}
	}

	
	void Renderer3D::DrawTriangle(const std::array<std::tuple<glm::vec3, glm::vec3, glm::vec2>, 3> & posNormalUV, glm::vec4 color, bool isLighted)
	{
		if (s_Data.triangleCount >= c_MaxTriangleCount)
			FlushTriangle();

		for (int i = 0; i < 3; i++)
		{
			auto [pos, normal, uv] = posNormalUV[i];
			s_Data.atTriangle->pos_IsLighted = glm::vec4(pos, isLighted ? 1.0f : 0.0f);
			s_Data.atTriangle->normal = normal;
			s_Data.atTriangle->uv = uv;
			s_Data.atTriangle->color = color;
			s_Data.atTriangle->texID = 0.0f;
			s_Data.atTriangle++;
		}

		s_Data.triangleCount++;
		s_Data.stats.triangleCount++;
	}
	void Renderer3D::DrawTriangle(const std::array<std::tuple<glm::vec3, glm::vec3, glm::vec2>, 3> & posNormalUV, Ref<Texture2D> texture, bool isLighted)
	{
		if (s_Data.triangleCount >= c_MaxTriangleCount || s_Data.texIndexTriangle > c_MaxTextures - 1)
			FlushTriangle();

		float tid = 0.0f;
		for (uint8_t i = 1; i < s_Data.texIndexTriangle; i++)
		{
			if (s_Data.texturesTriangle[i] == texture)
			{
				tid = (float)i;
				break;
			}
		}
		if (tid == 0.0f)
		{
			tid = (float)s_Data.texIndexTriangle;
			s_Data.texturesTriangle[s_Data.texIndexTriangle] = texture;
			s_Data.texIndexTriangle++;
		}

		for (int i = 0; i < 3; i++)
		{
			auto [pos, normal, uv] = posNormalUV[i];
			s_Data.atTriangle->pos_IsLighted = glm::vec4(pos, isLighted ? 1.0f : 0.0f);
			s_Data.atTriangle->normal = normal;
			s_Data.atTriangle->uv = uv;
			s_Data.atTriangle->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_Data.atTriangle->texID = tid;
			s_Data.atTriangle++;
		}

		s_Data.triangleCount++;
		s_Data.stats.triangleCount++;
	}

	void Renderer3D::DrawQuad(const std::array<std::pair<glm::vec3, glm::vec3>, 4> & posNormal, glm::vec4 color, bool isLighted)
	{
		if (s_Data.triangleCount >= c_MaxTriangleCount)
			FlushQuad();

		for (int i = 0; i < 4; i++)
		{
			s_Data.atQuad->pos_IsLighted = glm::vec4(posNormal[i].first, isLighted ? 1.0f : 0.0f);
			s_Data.atQuad->normal = posNormal[i].second;
			s_Data.atQuad->color = color;
			s_Data.atQuad->texID = 0.0f;
			s_Data.atQuad++;
		}

		s_Data.quadCount++;
		s_Data.stats.quadCount++;
	}
	void Renderer3D::DrawQuad(const std::array<std::pair<glm::vec3, glm::vec3>, 4> & posNormal, Ref<Texture2D> texture, bool isLighted)
	{
		if (s_Data.quadCount >= c_MaxQuadCount || s_Data.texIndexQuad > c_MaxTextures - 1)
			FlushQuad();

		float tid = 0.0f;
		for (uint8_t i = 1; i < s_Data.texIndexQuad; i++)
		{
			if (s_Data.texturesQuad[i] == texture)
			{
				tid = (float)i;
				break;
			}
		}
		if (tid == 0.0f)
		{
			tid = (float)s_Data.texIndexQuad;
			s_Data.texturesQuad[s_Data.texIndexQuad] = texture;
			s_Data.texIndexQuad++;
		}

		for (int i = 0; i < 4; i++)
		{
			s_Data.atQuad->pos_IsLighted = glm::vec4(posNormal[i].first, isLighted ? 1.0f : 0.0f);
			s_Data.atQuad->normal = posNormal[i].second;
			s_Data.atQuad->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_Data.atQuad->texID = tid;
			s_Data.atQuad++;
		}

		s_Data.quadCount++;
		s_Data.stats.quadCount++;
	}

	void Renderer3D::DrawModel(uint8_t modelID, glm::vec3 pos, glm::vec3 dir, glm::vec3 scale, glm::vec4 color, const std::vector<Ref<Texture2D>>& textures, bool isLighted)
	{
		if (s_Data.modelData[modelID].instancesCount >= c_MaxModelInstances || s_Data.modelData[modelID].texIndex + textures.size() >= c_MaxTextures)
			FlushModel(modelID);

		const float angle = glm::acos(glm::dot(dir, {1.0f, 0.0f, 0.0f}));
		const glm::vec3 axis = dir == glm::vec3(1.0f, 0.0f, 0.0f) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::normalize(glm::cross({ 1.0f, 0.0f, 0.0f }, dir));
		const glm::mat4 modelMat = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), pos), angle, axis), scale);

		float tid = 0.0f;
		if (textures[0])
		{
			auto it = std::search(s_Data.modelData[modelID].textures.begin(), s_Data.modelData[modelID].textures.end(), textures.begin(), textures.end());
			if (it != s_Data.modelData[modelID].textures.end())
			{
				tid = (float)(it - s_Data.modelData[modelID].textures.begin());
			}
			else
			{
				tid = (float)s_Data.modelData[modelID].texIndex;
				std::copy(textures.begin(), textures.end(), s_Data.modelData[modelID].textures.begin() + s_Data.modelData[modelID].texIndex);
				s_Data.modelData[modelID].texIndex += (uint8_t)textures.size();
			}
		}

		s_Data.modelData[modelID].at->modelMat = modelMat;
		s_Data.modelData[modelID].at->color = color;
		s_Data.modelData[modelID].at->tid_IsLighted = glm::vec2(tid, isLighted ? 1.0f : 0.0f);
		s_Data.modelData[modelID].at++;
		s_Data.modelData[modelID].instancesCount++;

		s_Data.stats.modelStats[modelID].timesDrawed++;
	}
	void Renderer3D::DrawModel(uint8_t modelID, glm::mat4 modelMat, glm::vec4 color, const std::vector<Ref<Texture2D>>& textures, bool isLighted)
	{
		if (s_Data.modelData[modelID].instancesCount >= c_MaxModelInstances || s_Data.modelData[modelID].texIndex + textures.size() >= c_MaxTextures)
			FlushModel(modelID);

		float tid = 0.0f;
		if (!textures.empty() && textures[0])
		{
			auto it = std::search(s_Data.modelData[modelID].textures.begin(), s_Data.modelData[modelID].textures.end(), textures.begin(), textures.end());
			if (it != s_Data.modelData[modelID].textures.end())
			{
				tid = (float)(it - s_Data.modelData[modelID].textures.begin());
			}
			else
			{
				tid = (float)s_Data.modelData[modelID].texIndex;
				std::copy(textures.begin(), textures.end(), s_Data.modelData[modelID].textures.begin() + s_Data.modelData[modelID].texIndex);
				s_Data.modelData[modelID].texIndex += (uint8_t)textures.size();
			}
		}

		s_Data.modelData[modelID].at->modelMat = modelMat;
		s_Data.modelData[modelID].at->color = color;
		s_Data.modelData[modelID].at->tid_IsLighted = glm::vec2(tid, isLighted ? 1.0f : 0.0f);
		s_Data.modelData[modelID].at++;
		s_Data.modelData[modelID].instancesCount++;

		s_Data.stats.modelStats[modelID].timesDrawed++;
	}

	void Renderer3D::ResetStats()
	{
		s_Data.stats.triangleCount = 0;
		s_Data.stats.drawCallsOnTriangles = 0;

		s_Data.stats.quadCount = 0;
		s_Data.stats.drawCallsOnQuads = 0;

		for (auto& data : s_Data.stats.modelStats)
		{
			data.timesDrawed = 0;
		}
	}
	const Renderer3D::Stats& Renderer3D::GetStats()
	{
		return s_Data.stats;
	}

}
