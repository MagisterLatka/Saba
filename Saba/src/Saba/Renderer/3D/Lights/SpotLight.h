#pragma once
#include "Light.h"

namespace Saba {

	class SpotLight : public Light
	{
	public:
		SpotLight(uint32_t objectID, glm::vec3 pos, glm::vec3 dir, float cutOffD, float outerCutOffD, float range, glm::vec3 diffuseColor = { 1.0f, 1.0f, 1.0f },
				  glm::vec3 specularColor = { 1.0f, 1.0f, 1.0f });
		SpotLight(Scene3DObject* object, glm::vec3 pos, glm::vec3 dir, float cutOffD, float outerCutOffD, float range, glm::vec3 diffuseColor = { 1.0f, 1.0f, 1.0f },
				  glm::vec3 specularColor = { 1.0f, 1.0f, 1.0f });
		~SpotLight() = default;

		virtual void SetPos(glm::vec3 pos) override;
		virtual void SetDir(glm::vec3 dir) override;
		virtual void SetDiffuseColor(glm::vec3 color) override;
		virtual void SetSpecularColor(glm::vec3 color) override;

		virtual const glm::mat4* SetShadowData(const std::pair<glm::vec2, glm::vec2>& shadowTextureSpace) override;

		virtual LightData* GetData() override;
		virtual LightData* GetData(LightData* bufferPtr) override;

		inline float GetOuterCutOff() const { return m_OuterCutOff; }
		inline float GetFarPlane() const { return m_FarPlane; }
	private:
		void SetLightSpace();
	private:
		glm::mat4 m_LightSpace;
		const float m_FarPlane;
	};
}
