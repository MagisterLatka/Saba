#include "pch.h"
#include "PointLight.h"
#include <glm\gtc\matrix_transform.hpp>

namespace Saba {

	PointLight::PointLight(uint32_t objectID, glm::vec3 pos, float range, glm::vec3 diffuseColor, glm::vec3 specularColor)
		: Light(objectID), m_FarPlane(range)
	{
		m_Pos = pos;
		m_DiffuseColor = diffuseColor;
		m_SpecularColor = specularColor;
		m_AttConstant = 1.0f;
		m_AttLinear = 4.5f / range;
		m_AttQuadratic = 75.0f / (range * range);
		SetLightSpace();
	}
	PointLight::PointLight(Scene3DObject* object, glm::vec3 pos, float range, glm::vec3 diffuseColor, glm::vec3 specularColor)
		: Light(object), m_FarPlane(range)
	{
		m_Pos = pos;
		m_DiffuseColor = diffuseColor;
		m_SpecularColor = specularColor;
		m_AttConstant = 1.0f;
		m_AttLinear = 4.5f / range;
		m_AttQuadratic = 75.0f / (range * range);
		SetLightSpace();
	}

	void PointLight::SetPos(glm::vec3 pos)
	{
		m_Pos = pos;
		SetLightSpace();
	}
	void PointLight::SetDiffuseColor(glm::vec3 color)
	{
		m_DiffuseColor = color;
	}
	void PointLight::SetSpecularColor(glm::vec3 color)
	{
		m_SpecularColor = color;
	}

	const glm::mat4* PointLight::SetShadowData(const std::pair<glm::vec2, glm::vec2>& shadowTextureSpace)
	{
		m_ShadowTextureSpace = shadowTextureSpace;

		return m_LightSpace;
	}

	Light::LightData* PointLight::GetData()
	{
		LightData* data = new LightData;
		data->pos = glm::vec4(m_Pos, 1.0f);
		data->dir = glm::vec4(0.0f);
		data->diffuseColor = glm::vec4(m_DiffuseColor, 0.0f);
		data->specularColor = glm::vec4(m_SpecularColor, 0.0f);
		data->cutsoff_FarPlane = glm::vec4(0.0f, 0.0f, m_FarPlane, 0.0f);
		data->attenuation = glm::vec4(m_AttConstant, m_AttLinear, m_AttQuadratic, 0.0f);
		data->shadowTextureSpace = glm::vec4(m_ShadowTextureSpace.first, m_ShadowTextureSpace.second);
		data->lightSpace = glm::mat4(1.0f);
		return data;
	}
	Light::LightData* PointLight::GetData(LightData* bufferPtr)
	{
		bufferPtr->pos = glm::vec4(m_Pos, 1.0f);
		bufferPtr->dir = glm::vec4(0.0f);;
		bufferPtr->diffuseColor = glm::vec4(m_DiffuseColor, 0.0f);
		bufferPtr->specularColor = glm::vec4(m_SpecularColor, 0.0f);
		bufferPtr->cutsoff_FarPlane = glm::vec4(0.0f, 0.0f, m_FarPlane, 0.0f);;
		bufferPtr->attenuation = glm::vec4(m_AttConstant, m_AttLinear, m_AttQuadratic, 0.0f);
		bufferPtr->shadowTextureSpace = glm::vec4(m_ShadowTextureSpace.first, m_ShadowTextureSpace.second);
		bufferPtr->lightSpace = glm::mat4(1.0f);
		return bufferPtr;
	}

	void PointLight::SetLightSpace()
	{
		m_LightSpace[0] = glm::lookAt(m_Pos, m_Pos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_LightSpace[1] = glm::lookAt(m_Pos, m_Pos + glm::vec3(0.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}
