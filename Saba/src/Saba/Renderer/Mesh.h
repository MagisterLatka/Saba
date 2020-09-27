#pragma once

#include "Saba/Core.h"
#include "Saba/Renderer/Texture.h"

namespace Saba {

	class Mesh
	{
		friend class Renderer3D;
		friend class SceneHierarchyPanel;
	public:
		struct VertexData
		{
			glm::vec3 pos;
			glm::vec4 normal_Use;
			glm::vec4 tangent_Use;
			glm::vec2 uv;
			glm::vec4 color;
		};
		enum MeshType
		{
			Position = BIT(0),
			Normal = BIT(1),
			Tangent = BIT(2),
			UV = BIT(3),
			Color = BIT(4)
		};
	public:
		Mesh() = default;
		Mesh(float* vertexData, uint32_t verticesCount, MeshType type = MeshType::Position, uint32_t* indices = nullptr, uint32_t indicesCount = 0);
		Mesh(const std::vector<VertexData>& vertexData, MeshType type = MeshType::Position, const std::vector<uint32_t>& indices = {});

		void Reload(float* vertexData, uint32_t verticesCount, MeshType type = MeshType::Position, uint32_t* indices = nullptr, uint32_t indicesCount = 0);
		void Reload(const std::vector<VertexData>& vertexData, MeshType type = MeshType::Position, const std::vector<uint32_t>& indices = {});

		const std::vector<VertexData>& GetVertexData() const { return m_VertexData; }
		uint32_t GetVerticesCount() const { return (uint32_t)m_VertexData.size(); }
		const std::vector<uint32_t>& GetIndices() const { return m_Indices; }
		uint32_t GetIndicesCount() const { return (uint32_t)m_Indices.size(); }
		MeshType GetMeshType() const { return m_MeshType; }

		void SetAlbedoTex(const Ref<Texture2D>& texture) { m_AlbedoTex = texture; }
		Ref<Texture2D> GetAlbedoTex() const { return m_AlbedoTex; }
		void SetNormalTex(const Ref<Texture2D>& texture) { m_NormalTex = texture; }
		Ref<Texture2D> GetNormalTex() const { return m_NormalTex; }
		void SetHeightTex(const Ref<Texture2D>& texture) { m_HeightTex = texture; }
		Ref<Texture2D> GetHeightTex() const { return m_HeightTex; }
		void SetRoughnessTex(const Ref<Texture2D>& texture) { m_RoughnessTex = texture; }
		Ref<Texture2D> GetRoughnessTex() const { return m_RoughnessTex; }
		void SetMetallicTex(const Ref<Texture2D>& texture) { m_MetallicTex = texture; }
		Ref<Texture2D> GetMetallicTex() const { return m_MetallicTex; }
		void SetAOTex(const Ref<Texture2D>& texture) { m_AOTex = texture; }
		Ref<Texture2D> GetAOTex() const { return m_AOTex; }

		void SetTextures(const Ref<Texture2D>& albedoTex, const Ref<Texture2D>& normalTex = {}, const Ref<Texture2D>& heightTex = {}, const Ref<Texture2D>& roughnessTex = {},
						 const Ref<Texture2D>& metallicTex = {}, const Ref<Texture2D>& aoTex = {})
		{
			m_AlbedoTex = albedoTex;
			m_NormalTex = normalTex;
			m_HeightTex = heightTex;
			m_RoughnessTex = roughnessTex;
			m_MetallicTex = metallicTex;
			m_AOTex = aoTex;
		}
		void SetTextures(const std::vector<Ref<Texture2D>>& textures)
		{
			for (int i = 0; i < textures.size() && i < 6; i++)
			{
				switch (i)
				{
					case 0:
						m_AlbedoTex = textures[i];
						break;
					case 1:
						m_NormalTex = textures[i];
						break;
					case 2:
						m_HeightTex = textures[i];
						break;
					case 3:
						m_RoughnessTex = textures[i];
						break;
					case 4:
						m_MetallicTex = textures[i];
						break;
					case 5:
						m_AOTex = textures[i];
						break;
				}
			}
		}
	private:
		uint32_t m_MeshID = 4294967295;

		std::string m_Filepath;
		MeshType m_MeshType;
		std::vector<VertexData> m_VertexData;
		std::vector<uint32_t> m_Indices;

		Ref<Texture2D> m_AlbedoTex;
		Ref<Texture2D> m_NormalTex;
		Ref<Texture2D> m_HeightTex;
		Ref<Texture2D> m_RoughnessTex;
		Ref<Texture2D> m_MetallicTex;
		Ref<Texture2D> m_AOTex;

		static uint32_t s_MaxVertices;
		static uint32_t s_MaxIndices;
	};
}
