#include "pch.h"
#include "Scene3D.h"

namespace Saba {

	Scene3D::Scene3D()
	{
		m_Lights.fill(nullptr);
	}
	Scene3D::~Scene3D()
	{}

	Scene3DObject* Scene3D::Add(Scene3DObject * object)
	{
		object->m_ID = (uint32_t)m_Objects.size();
		m_Objects.push_back(object);
		return object;
	}
	Scene3DObject* Scene3D::Delete(Scene3DObject* object)
	{
		auto i = std::find(m_Objects.begin(), m_Objects.end(), object);
		if (!*i)
		{
			SB_CORE_ASSERT(false, "There is no such object");
		}
		Scene3DObject* a = *i;
		m_Objects.erase(i);
		return a;
	}

	Scene3DObject* Scene3D::Delete(uint32_t id)
	{
		SB_CORE_ASSERT((m_Objects.size() > id), "There is no object with id {0}", id);
		Scene3DObject* a = m_Objects[id];
		m_Objects.erase(m_Objects.begin() + id);
		return a;
	}

	uint32_t Scene3D::GetID(Scene3DObject* object)
	{
		auto i = std::find(m_Objects.begin(), m_Objects.end(), object);
		if (!*i)
		{
			SB_CORE_ASSERT(false, "There is no such object");
		}
		return (uint32_t)(i - m_Objects.begin());
	}

	void Scene3D::Draw(uint32_t id)
	{
		SB_CORE_ASSERT((m_Objects.size() > id), "There is no object with id {0}", id);
		m_Objects[id]->Draw();
	}
	void Scene3D::DrawAll()
	{
		for (auto& o : m_Objects)
		{
			o->Draw();
		}
	}

	Light* Scene3D::AddLight(Light* light)
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
		return light;
	}
	Light* Scene3D::DeleteLight(Light* light)
	{
		auto i = std::find(m_Lights.begin(), m_Lights.end(), light);
		if (!*i)
		{
			SB_CORE_ASSERT(false, "There is no such light");
		}
		Light* a = *i;
		*i = nullptr;
		return a;
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
	uint32_t Scene3D::GetLightID(Light* light)
	{
		auto i = std::find(m_Lights.begin(), m_Lights.end(), light);
		if (!*i)
		{
			SB_CORE_ASSERT(false, "There is no such light");
		}
		return (uint32_t)(i - m_Lights.begin());
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
