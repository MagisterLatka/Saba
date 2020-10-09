#pragma once

#include "Saba/Renderer/Mesh.h"

namespace Saba {

	class Model
	{
	public:
		Model() = default;
		Model(const std::vector<Ref<Mesh>>& meshes);
		Model(const Ref<Mesh>& mesh);
		Model(const std::string& filepath);

		void Load(const std::string& filepath);

		void AddMesh(const Ref<Mesh>& mesh);
		Ref<Mesh> GetMesh(uint32_t id) const { return m_Meshes[id]; }
		const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }
		Ref<Mesh> operator[](uint32_t id) const { return m_Meshes[id]; }

		std::vector<Ref<Mesh>>::iterator begin() { return m_Meshes.begin(); }
		std::vector<Ref<Mesh>>::iterator end() { return m_Meshes.end(); }
		std::vector<Ref<Mesh>>::reverse_iterator rbegin() { return m_Meshes.rbegin(); }
		std::vector<Ref<Mesh>>::reverse_iterator rend() { return m_Meshes.rend(); }

		std::vector<Ref<Mesh>>::const_iterator begin() const { return m_Meshes.begin(); }
		std::vector<Ref<Mesh>>::const_iterator end() const { return m_Meshes.end(); }
		std::vector<Ref<Mesh>>::const_reverse_iterator regin() const { return m_Meshes.rbegin(); }
		std::vector<Ref<Mesh>>::const_reverse_iterator rend() const { return m_Meshes.rend(); }
	private:
		std::vector<Ref<Mesh>> m_Meshes;
	};
}
