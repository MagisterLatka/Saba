#include "pch.h"
#include "Scene3D.h"

namespace Saba {

	Scene3D::Scene3D()
	{}
	Scene3D::~Scene3D()
	{}

	void Scene3D::Add(Scene3DObject * object)
	{
		object->m_ID = (uint32_t)m_Objects.size();
		m_Objects.push_back(object);
	}

	void Scene3D::Draw(uint32_t id)
	{
		SB_CORE_ASSERT((m_Objects.size() > id), "There is no object with id {0}", id);
		m_Objects[id]->Draw();
	}
	void Scene3D::DrawAll()
	{
		for (auto& o : m_Objects)
			o->Draw();
	}
}
