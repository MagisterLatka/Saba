#include "pch.h"
#include "Mesh.h"

namespace Saba {

	uint32_t Mesh::s_MaxVertices = 0;
	uint32_t Mesh::s_MaxIndices = 0;

	Mesh::Mesh(float* vertexData, uint32_t verticesCount, MeshType type, uint32_t* indices, uint32_t indicesCount)
	{
		Reload(vertexData, verticesCount, type, indices, indicesCount);
	}
	Mesh::Mesh(const std::vector<VertexData>& vertexData, MeshType type, const std::vector<uint32_t>& indices)
	{
		Reload(vertexData, type, indices);
	}

	void Mesh::Reload(float* vertexData, uint32_t verticesCount, MeshType type, uint32_t* indices, uint32_t indicesCount)
	{
		if (verticesCount > s_MaxVertices)
		{
			SB_CORE_ASSERT(false, "Vertices limit extended!");
			return;
		}
		if (indicesCount > s_MaxIndices)
		{
			SB_CORE_ASSERT(false, "Indices limit extended");
			return;
		}
		if (type == 0 || type >= MeshType::Color * 2 || !(type & MeshType::Position))
		{
			SB_CORE_ASSERT(false, "Invalid mesh type");
			return;
		}
		m_MeshType = type;

		m_VertexData.clear();
		m_VertexData.resize(verticesCount);
		m_Indices.clear();
		m_Indices.resize(indicesCount);

		float* at = vertexData;
		for (uint32_t i = 0; i < verticesCount; i++)
		{
			auto& vertex = m_VertexData[i];
			if (type & MeshType::Position)
			{
				vertex.pos = *(glm::vec3*)at;
				at += 3;
			}

			if (type & MeshType::Normal)
			{
				vertex.normal_Use = glm::vec4(*(glm::vec3*)at, 1.0f);
				at += 3;
			}
			else vertex.normal_Use.w = 0.0f;

			if (type & MeshType::Tangent)
			{
				vertex.tangent_Use = glm::vec4(*(glm::vec3*)at, 1.0f);
				at += 3;
			}
			else vertex.tangent_Use.w = 0.0f;

			if (type & MeshType::UV)
			{
				vertex.uv = *(glm::vec2*)at;
				at += 2;
			}
			else vertex.uv = { 0.0f, 0.0f };

			if (type & MeshType::Color)
			{
				vertex.color = *(glm::vec4*)at;
				at += 4;
			}
			else vertex.color = { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		if (indicesCount)
		{
			memcpy(&m_Indices[0], indices, indicesCount * sizeof(uint32_t));
		}
	}
	void Mesh::Reload(const std::vector<VertexData>& vertexData, MeshType type, const std::vector<uint32_t>& indices)
	{
		if (vertexData.size() > s_MaxVertices)
		{
			SB_CORE_ASSERT(false, "Vertices limit extended!");
			return;
		}
		if (indices.size() > s_MaxIndices)
		{
			SB_CORE_ASSERT(false, "Indices limit extended");
			return;
		}
		if (type == 0 || type >= MeshType::Color * 2 || !(type & MeshType::Position))
		{
			SB_CORE_ASSERT(false, "Invalid mesh type");
			return;
		}
		m_MeshType = type;

		m_VertexData = vertexData;
		m_Indices = indices;
	}
}
