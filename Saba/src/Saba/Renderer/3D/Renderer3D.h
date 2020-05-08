#pragma once

#include "Saba\Renderer\Texture.h"
#include "Saba\Renderer\VertexArray.h"
#include "Saba\Renderer\RendererAPI.h"

namespace Saba {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();
		static void Flush();

		static void BeginSceneTriangle();
		static void EndSceneTriangle();
		static void FlushQuad();

		static void BeginSceneQuad();
		static void EndSceneQuad();
		static void FlushTriangle();

		static void BeginSceneModel(uint8_t modelID);
		static void EndSceneModel(uint8_t modelID);
		static void FlushModel(uint8_t modelID);

		static void DrawTriangle(const std::array<std::tuple<glm::vec3, glm::vec3, glm::vec2>, 3> & posUV, glm::vec4 color);
		static void DrawTriangle(const std::array<std::tuple<glm::vec3, glm::vec3, glm::vec2>, 3> & posUV, Ref<Texture2D> texture);

		static void DrawQuad(const std::array<std::pair<glm::vec3, glm::vec3>, 4> & pos, glm::vec4 color);
		static void DrawQuad(const std::array<std::pair<glm::vec3, glm::vec3>, 4> & pos, Ref<Texture2D> texture);

		template<class T>
		inline static void AddModel(RendererAPI::RenderTopology topology, const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec2, glm::vec4, float>>& posNormalUVColorTID, const std::vector<uint32_t>& indices, const std::vector<Ref<Texture2D>>& textures)
		{
			SB_CORE_ASSERT((textures.size() < c_MaxTextures - 1), "Max textures limit extented");

			uint8_t id = (uint8_t)s_Data.modelData.size();
			T::SetModelID(id);
			s_Data.modelData.emplace_back();
			s_Data.modelData[id].topology = topology;
			s_Data.modelData[id].vertexArray = VertexArray::Create();

			s_Data.modelData[id].modelMatBuffer = new ModelVertexBuffer[c_MaxModelInstances];
			s_Data.modelData[id].at = s_Data.modelData[id].modelMatBuffer;

			VertexData* data = (VertexData*)malloc(posNormalUVColorTID.size() * sizeof(VertexData));
			VertexData* at = data;
			for (auto [pos, normal, uv, color, tid] : posNormalUVColorTID)
			{
				at->pos = pos;
				at->normal = normal;
				at->uv = uv;
				at->color = color;
				at->texID = tid;
				at++;
			}
			Ref<VertexBuffer> vbo = VertexBuffer::Create((float*)data, (uint32_t)(posNormalUVColorTID.size() * sizeof(VertexData)));
			free(data);
			vbo->SetLayout({
				{ "i_Pos", ShaderDataType::Float3 },
				{ "i_Normal", ShaderDataType::Float3 },
				{ "i_UV", ShaderDataType::Float2 },
				{ "i_Color", ShaderDataType::Float4 },
				{ "i_TID", ShaderDataType::Float }
			});
			s_Data.modelData[id].vertexArray->AddVertexBuffer(vbo);

			vbo = VertexBuffer::Create(nullptr, c_MaxModelInstances * sizeof(ModelData), Dynamic);
			vbo->SetLayout({
				{ "i_ModelMat", ShaderDataType::Mat4, 1 },
				{ "i_ColorMul", ShaderDataType::Float4, 1 },
				{ "i_TIDoptional", ShaderDataType::Float, 1 }
			});
			s_Data.modelData[id].vertexArray->AddVertexBuffer(vbo);

			uint32_t* ind = (uint32_t*)malloc(indices.size() * sizeof(uint32_t));
			uint32_t* indAt = ind;
			for (auto& index : indices)
			{
				*indAt = index;
				indAt++;
			}
			Ref<IndexBuffer> ibo = IndexBuffer::Create(ind, (uint32_t)indices.size());
			free(ind);
			s_Data.modelData[id].vertexArray->SetIndexBuffer(ibo);

			s_Data.modelData[id].textures[0] = s_Data.whiteTex;
			for (int i = 0; i < textures.size(); i++)
				s_Data.modelData[id].textures[i + 1] = textures[i];
			s_Data.modelData[id].texIndex = (uint8_t)(textures.size()) + 1;

			s_Data.stats.modelStats.emplace_back();
			s_Data.stats.modelStats[id].verticesCount = (uint32_t)posNormalUVColorTID.size();
			s_Data.stats.modelStats[id].indicesCount = (uint32_t)indices.size();
		}
		static void DrawModel(uint8_t modelID, glm::vec3 pos, glm::vec3 dir = { 1.0f, 0.0f, 0.0f }, glm::vec3 scale = { 1.0f, 1.0f, 1.0f }, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, const std::vector<Ref<Texture2D>>& textures = {});
		static void DrawModel(uint8_t modelID, glm::mat4 modelMat, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, const std::vector<Ref<Texture2D>>& textures = {});

		struct Stats
		{
			uint32_t triangleCount = 0;
			uint32_t drawCallsOnTriangles = 0;

			uint32_t quadCount = 0;
			uint32_t drawCallsOnQuads = 0;

			struct ModelStat
			{
				uint32_t verticesCount = 0;
				uint32_t indicesCount = 0;
				uint32_t timesDrawed = 0;
			};
			std::vector<ModelStat> modelStats = {};
		};

		static void ResetStats();
		static const Stats& GetStats();
	private:
		constexpr static uint32_t c_MaxTriangleCount = 12000;
		constexpr static uint32_t c_MaxQuadCount = 9000;
		constexpr static uint32_t c_MaxModelInstances = 100;
		constexpr static uint8_t c_MaxTextures = 32;
		struct VertexData
		{
			glm::vec3 pos;
			glm::vec3 normal;
			glm::vec2 uv;
			glm::vec4 color;
			float texID;
		};
		struct ModelVertexBuffer
		{
			glm::mat4 modelMat;
			glm::vec4 color;
			float tid;
		};

		struct ModelData
		{
			Ref<VertexArray> vertexArray;
			uint32_t instancesCount;

			ModelVertexBuffer* modelMatBuffer;
			ModelVertexBuffer* at;

			std::array<Ref<Texture2D>, c_MaxTextures> textures;
			uint8_t texIndex = 1;

			RendererAPI::RenderTopology topology;
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

			Ref<Texture2D> whiteTex;
			std::array<Ref<Texture2D>, c_MaxTextures> texturesTriangle;
			std::array<Ref<Texture2D>, c_MaxTextures> texturesQuad;
			uint8_t texIndexTriangle = 1;
			uint8_t texIndexQuad = 1;

			std::vector<ModelData> modelData;

			Renderer3D::Stats stats;
		};
		static Renderer3DData s_Data;
	};
}
