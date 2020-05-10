#include "pch.h"
#include "SpotLight.h"

namespace Saba {

	SpotLight::SpotLight(glm::vec3 pos, glm::vec3 dir, float cutOff, float outerCutOff, float range, glm::vec3 diffuseColor, glm::vec3 specularColor)
	{
		m_Pos = pos;
		m_Dir = dir;
		m_CutOff = glm::cos(glm::radians(cutOff));
		m_OuterCutOff = glm::cos(glm::radians(outerCutOff));
		m_DiffuseColor = diffuseColor;
		m_SpecularColor = specularColor;
		m_AttConstant = 1.0f;
		m_AttLinear = 4.5f / range;
		m_AttQuadratic = 75.0f / (range * range);
	}

	void SpotLight::SetPos(glm::vec3 pos)
	{
		m_Pos = pos;
	}
	void SpotLight::SetDir(glm::vec3 dir)
	{
		m_Dir = dir;
	}
	void SpotLight::SetDiffuseColor(glm::vec3 color)
	{
		m_DiffuseColor = color;
	}
	void SpotLight::SetSpecularColor(glm::vec3 color)
	{
		m_SpecularColor = color;
	}

	Light::LightData* SpotLight::GetData()
	{
		LightData* data = new LightData;
		data->pos = glm::vec4(m_Pos, 1.0f);
		data->dir = glm::vec4(m_Dir, 1.0f);
		data->diffuseColor = m_DiffuseColor;
		data->specularColor = m_SpecularColor;
		data->cutsoff = glm::vec2(m_CutOff, m_OuterCutOff);
		data->attenuation = glm::vec3(m_AttConstant, m_AttLinear, m_AttQuadratic);
		return data;
	}
	Light::LightData* SpotLight::GetData(LightData* bufferPtr)
	{
		bufferPtr->pos = glm::vec4(m_Pos, 1.0f);
		bufferPtr->dir = glm::vec4(m_Dir, 1.0f);
		bufferPtr->diffuseColor = m_DiffuseColor;
		bufferPtr->specularColor = m_SpecularColor;
		bufferPtr->cutsoff = glm::vec2(m_CutOff, m_OuterCutOff);
		bufferPtr->attenuation = glm::vec3(m_AttConstant, m_AttLinear, m_AttQuadratic);
		return bufferPtr;
	}
}
