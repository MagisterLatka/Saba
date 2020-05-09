#include "pch.h"
#include "Scene3D.h"

namespace Saba {

	Scene3D::Scene3D()
	{
		m_Lights.fill(nullptr);
	}
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
	void Scene3D::DrawAllLighted()
	{
		for (auto& o : m_Objects)
		{
			if (o->m_IsLighted)
				o->Draw();
		}
	}
	void Scene3D::DrawAllNotLighted()
	{
		for (auto& o : m_Objects)
		{
			if (!o->m_IsLighted)
				o->Draw();
		}
	}

	void Scene3D::AddLight(Light* light)
	{
		bool hasSpace = false;
		uint8_t emptySlot = 255;
		for (uint8_t i = 0; i < c_MaxLights; i++)
		{
			if (!m_Lights[i])
			{
				hasSpace = true;
				emptySlot = i;
				break;
			}
		}
		SB_CORE_ASSERT(hasSpace, "Max lights number extended");
		m_Lights[emptySlot] = light;
	}
	Light* Scene3D::DeleteLight(uint8_t id)
	{
		Light* light = nullptr;
		if (id < c_MaxLights)
		{
			light = m_Lights[id];
			m_Lights[id] = nullptr;
		}
		return light;
	}
	Light::LightData* Scene3D::GetLightsData()
	{
		uint32_t size = 0;
		for (uint8_t i = 0; i < c_MaxLights; i++)
		{
			if (m_Lights[i])
				size += sizeof(Light::LightData);
		}

		if (size == 0) return nullptr;

		Light::LightData* data = (Light::LightData*)malloc(size);
		for (uint8_t i = 0, a = 0; i < c_MaxLights; i++)
		{
			if (m_Lights[i])
				m_Lights[i]->GetData(&data[i]);
		}
		return data;
	}
	Light::LightData* Scene3D::GetLightsData(Light::LightData* bufferPtr)
	{
		uint32_t size = 0;
		for (uint8_t i = 0; i < c_MaxLights; i++)
		{
			if (m_Lights[i])
				size += sizeof(Light::LightData);
		}

		if (size == 0) return nullptr;

		for (uint8_t i = 0, a = 0; i < c_MaxLights; i++)
		{
			if (m_Lights[i])
				m_Lights[i]->GetData(&bufferPtr[i]);
		}

		return bufferPtr;
	}
}
