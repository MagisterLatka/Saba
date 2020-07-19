#include "pch.h"
#include "SpotLight.h"
#include <glm\gtc\matrix_transform.hpp>

namespace Saba {

	SpotLight::SpotLight(uint32_t objectID, glm::vec3 pos, glm::vec3 dir, float cutOff, float outerCutOff, float range, glm::vec3 diffuseColor, glm::vec3 specularColor)
		: Light(objectID), m_FarPlane(range)
	{
		m_Pos = pos;
		m_Dir = glm::normalize(dir);
		m_CutOff = glm::radians(cutOff);
		m_OuterCutOff = glm::radians(outerCutOff);
		m_DiffuseColor = diffuseColor;
		m_SpecularColor = specularColor;
		m_AttConstant = 1.0f;
		m_AttLinear = 4.5f / range;
		m_AttQuadratic = 75.0f / (range * range);
		SetLightSpace();
	}
	SpotLight::SpotLight(Scene3DObject* object, glm::vec3 pos, glm::vec3 dir, float cutOff, float outerCutOff, float range, glm::vec3 diffuseColor, glm::vec3 specularColor)
		: Light(object), m_FarPlane(range)
	{
		m_Pos = pos;
		m_Dir = glm::normalize(dir);
		m_CutOff = glm::radians(cutOff);
		m_OuterCutOff = glm::radians(outerCutOff);
		m_DiffuseColor = diffuseColor;
		m_SpecularColor = specularColor;
		m_AttConstant = 1.0f;
		m_AttLinear = 4.5f / range;
		m_AttQuadratic = 75.0f / (range * range);
		SetLightSpace();
	}

	void SpotLight::SetPos(glm::vec3 pos)
	{
		m_Pos = pos;
		SetLightSpace();
	}
	void SpotLight::SetDir(glm::vec3 dir)
	{
		m_Dir = glm::normalize(dir);
		SetLightSpace();
	}
	void SpotLight::SetDiffuseColor(glm::vec3 color)
	{
		m_DiffuseColor = color;
	}
	void SpotLight::SetSpecularColor(glm::vec3 color)
	{
		m_SpecularColor = color;
	}

	const glm::mat4* SpotLight::SetShadowData(const std::pair<glm::vec2, glm::vec2>& shadowTextureSpace)
	{
		m_ShadowTextureSpace = shadowTextureSpace;

		return &m_LightSpace;
	}

	Light::LightData* SpotLight::GetData()
	{
		LightData* data = new LightData;
		data->pos = glm::vec4(m_Pos, 1.0f);
		data->dir = glm::vec4(m_Dir, 1.0f);
		data->diffuseColor = glm::vec4(m_DiffuseColor, 0.0f);
		data->specularColor = glm::vec4(m_SpecularColor, 0.0f);
		data->cutsoff_FarPlane = glm::vec4(m_CutOff, m_OuterCutOff, m_FarPlane, 0.0f);
		data->attenuation = glm::vec4(m_AttConstant, m_AttLinear, m_AttQuadratic, 0.0f);
		data->shadowTextureSpace = glm::vec4(m_ShadowTextureSpace.first, m_ShadowTextureSpace.second);
		data->lightSpace = m_LightSpace;
		return data;
	}
	Light::LightData* SpotLight::GetData(LightData* bufferPtr)
	{
		bufferPtr->pos = glm::vec4(m_Pos, 1.0f);
		bufferPtr->dir = glm::vec4(m_Dir, 1.0f);
		bufferPtr->diffuseColor = glm::vec4(m_DiffuseColor, 0.0f);
		bufferPtr->specularColor = glm::vec4(m_SpecularColor, 0.0f);
		bufferPtr->cutsoff_FarPlane = glm::vec4(m_CutOff, m_OuterCutOff, m_FarPlane, 0.0f);
		bufferPtr->attenuation = glm::vec4(m_AttConstant, m_AttLinear, m_AttQuadratic, 0.0f);
		bufferPtr->shadowTextureSpace = glm::vec4(m_ShadowTextureSpace.first, m_ShadowTextureSpace.second);
		bufferPtr->lightSpace = m_LightSpace;
		return bufferPtr;
	}
	void SpotLight::SetLightSpace()
	{
		glm::vec3 up = (m_Dir == glm::vec3(0.0f, 1.0f, 0.0f)) || (m_Dir == glm::vec3(0.0f, -1.0f, 0.0f)) ? glm::vec3(1.0f, 0.0f, 0.0f) :
			glm::cross(glm::normalize(glm::cross(m_Dir, { 0.0f, 1.0f, 0.0f })), m_Dir);
		m_LightSpace = glm::lookAt(m_Pos, m_Pos + m_Dir, up);
	}
}
