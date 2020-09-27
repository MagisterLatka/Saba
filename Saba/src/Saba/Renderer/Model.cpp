#include "pch.h"
#include "Model.h"

namespace Saba {

	Model::Model(const std::vector<Ref<Mesh>>& meshes)
		: m_Meshes(meshes)
	{}
	Model::Model(const Ref<Mesh>& mesh)
		: m_Meshes({ mesh })
	{}

	void Model::AddMesh(const Ref<Mesh>& mesh)
	{
		m_Meshes.push_back(mesh);
	}
}
