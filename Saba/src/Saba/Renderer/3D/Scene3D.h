#pragma once

#include "Scene3DObject.h"
#include "Lights\Light.h"
#include "Saba\Renderer\Buffer.h"

namespace Saba {

	class Scene3D
	{
	public:
		Scene3D();
		Scene3D(const Scene3D&) = delete;
		Scene3D operator=(const Scene3D&) = delete;
		~Scene3D();

		void Add(Scene3DObject* object);
		inline Scene3DObject* Get(uint32_t id) const { return m_Objects[id]; }
		inline Scene3DObject* operator[](uint32_t id) const { return m_Objects[id]; }

		void Draw(uint32_t id);
		void DrawAllLighted();
		void DrawAllNotLighted();

		void AddLight(Light* light);
		Light* DeleteLight(uint8_t id);
		inline Light* GetLight(uint8_t id) const { if (id < c_MaxLights) return m_Lights[id]; return nullptr; }
		Light::LightData* GetLightsData();
		Light::LightData* GetLightsData(Light::LightData* bufferPtr);
	private:
		constexpr static uint8_t c_MaxLights = 10;
		std::vector<Scene3DObject*> m_Objects;
		std::array<Light*, c_MaxLights> m_Lights;
	};
}
