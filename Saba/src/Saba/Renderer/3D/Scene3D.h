#pragma once

#include "Scene3DObject.h"
#include "Lights\Light.h"
#include "Saba\Renderer\Framebuffer.h"
#include "Saba\Renderer\Texture.h"
#include "Saba\Renderer\Shader.h"

namespace Saba {

	class Scene3D
	{
	public:
		Scene3D();
		Scene3D(const Scene3D&) = delete;
		Scene3D operator=(const Scene3D&) = delete;
		~Scene3D();

		Scene3DObject* Add(Scene3DObject* object);
		Scene3DObject* Delete(Scene3DObject* object);
		Scene3DObject* Delete(uint32_t id);
		inline Scene3DObject* Get(uint32_t id) const { return m_Objects[id]; }
		inline Scene3DObject* operator[](uint32_t id) const { return m_Objects[id]; }
		uint32_t GetID(Scene3DObject* object);
		inline uint32_t GetCount() const { return (uint32_t)m_Objects.size(); };

		void Draw(uint32_t id);
		void DrawAll();

		Light* AddLight(Light* light);
		Light* DeleteLight(Light* light);
		Light* DeleteLight(uint8_t id);
		inline Light* GetLight(uint8_t id) const { if (id < c_MaxLights) return m_Lights[id]; return nullptr; }
		uint32_t GetLightID(Light* light);
		inline uint32_t GetLightsCount() const { return c_MaxLights; };
		Light::LightData* GetLightsData();
		Light::LightData* GetLightsData(Light::LightData* bufferPtr);

		void DrawShadows(Ref<Shader> dirShadowShader, Ref<Shader> pointShadowShader, Ref<Shader> spotShadowShader);
		void BindShadowTextures(uint8_t dirShadowTexIndex, uint8_t pointShadowTexIndex, uint8_t spotShadowTexIndex);
		void BindShadowTextures(glm::u8vec3 shadowTexIndexes);
	private:
		static constexpr uint8_t c_MaxLights = 10;
		std::vector<Scene3DObject*> m_Objects;
		std::array<Light*, c_MaxLights> m_Lights;

		static constexpr uint32_t c_ShadowTexResolution = 1024;
		static constexpr int c_MaxDirLights = 4, c_MaxPointLights = 4, c_MaxSpotLights = 4;
		Ref<Framebuffer> m_DirShadowFramebuffer, m_PointShadowFramebuffer, m_SpotShadowFramebuffer;
		Ref<Texture2D> m_DirShadowTexture, m_PointShadowTexture, m_SpotShadowTexture;
	};
}
