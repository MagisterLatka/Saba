#pragma once

#include "Light.h"

namespace Saba {

	class PointLight : public Light
	{
	public:
		PointLight(glm::vec3 pos, float range, glm::vec3 diffuseColor = { 1.0f, 1.0f, 1.0f }, glm::vec3 specularColor = { 1.0f, 1.0f, 1.0f });
		~PointLight() = default;

		virtual void SetPos(glm::vec3 pos) override;
		virtual void SetDir(glm::vec3 dir) override {}
		virtual void SetDiffuseColor(glm::vec3 color) override;
		virtual void SetSpecularColor(glm::vec3 color) override;

		virtual glm::mat4 SetShadowData(const std::pair<glm::vec2, glm::vec2>& shadowTextureSpace) override;

		virtual LightData* GetData() override;
		virtual LightData* GetData(LightData* bufferPtr) override;
	};
}
