#include "pch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Saba {

	static Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Mesh::VertexData> vertices;
		std::vector<uint32_t> indices;
		using mType = Mesh::MeshType;
		mType type = (mType)(mType::Position | mType::Normal * mesh->HasNormals() | mType::Tangent * mesh->HasTangentsAndBitangents() |
							 mType::UV * mesh->HasTextureCoords(0) | mType::Color * mesh->HasVertexColors(0));

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			Mesh::VertexData data;

			data.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			if (type & Mesh::MeshType::Normal) data.normal_Use = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 1.0f };
			else data.normal_Use.w = 0.0f;
			if (type & Mesh::MeshType::Tangent) data.tangent_Use = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1.0f };
			else data.tangent_Use.w = 0.0f;
			if (type & Mesh::MeshType::Color) data.color = { mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a };
			else data.color = { 1.0f, 1.0f, 1.0f, 1.0f };
			if (type & Mesh::MeshType::UV) data.uv = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			else data.uv = { 0.0f, 0.0f };

			vertices.emplace_back(data);
		}
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return MakeRef<Mesh>(vertices, type, indices);
	}
	static void ProcessNode(std::vector<Ref<Mesh>>& meshes, aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
			meshes.emplace_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
		for (uint32_t i = 0; i < node->mNumChildren; i++)
			ProcessNode(meshes, node->mChildren[i], scene);
	}

	Model::Model(const std::vector<Ref<Mesh>>& meshes)
		: m_Meshes(meshes)
	{}
	Model::Model(const Ref<Mesh>& mesh)
		: m_Meshes({ mesh })
	{}
	Model::Model(const std::string & filepath)
	{
		Load(filepath);
	}

	void Model::Load(const std::string & filepath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath, aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_Triangulate);
		SB_CORE_ASSERT(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode, "Failed to open model: {0}", importer.GetErrorString());
		m_Meshes.clear();
		ProcessNode(m_Meshes, scene->mRootNode, scene);
	}

	void Model::AddMesh(const Ref<Mesh>& mesh)
	{
		m_Meshes.push_back(mesh);
	}
}
