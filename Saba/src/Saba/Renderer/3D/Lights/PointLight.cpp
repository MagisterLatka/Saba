#include "pch.h"
#include "PointLight.h"

namespace Saba {

	PointLight::PointLight(glm::vec3 pos, float range, glm::vec3 diffuseColor, glm::vec3 specularColor)
	{
		m_Pos = pos;
		m_DiffuseColor = diffuseColor;
		m_SpecularColor = specularColor;
		m_AttConstant = 1.0f;
		m_AttLinear = 4.5f / range;
		m_AttQuadratic = 75.0f / (range * range);
	}

	void PointLight::SetPos(glm::vec3 pos)
	{
		m_Pos = pos;
	}
	void PointLight::SetDiffuseColor(glm::vec3 color)
	{
		m_DiffuseColor = color;
	}
	void PointLight::SetSpecularColor(glm::vec3 color)
	{
		m_SpecularColor = color;
	}

	Light::LightData* PointLight::GetData()
	{
		LightData* data = new LightData;
		data->pos = glm::vec4(m_Pos, 1.0f);
		data->dir.w = 0.0f;
		data->diffuseColor = m_DiffuseColor;
		data->speculatColor = m_SpecularColor;
		data->cutsoff = glm::vec2(0.0f);
		data->attenuation = glm::vec3(m_AttConstant, m_AttLinear, m_AttQuadratic);
		return data;
	}
	Light::LightData* PointLight::GetData(LightData* bufferPtr)
	{
		bufferPtr->pos = glm::vec4(m_Pos, 1.0f);
		bufferPtr->dir.w = 0.0f;
		bufferPtr->diffuseColor = m_DiffuseColor;
		bufferPtr->speculatColor = m_SpecularColor;
		bufferPtr->cutsoff = glm::vec2(0.0f);
		bufferPtr->attenuation = glm::vec3(m_AttConstant, m_AttLinear, m_AttQuadratic);
		return bufferPtr;
	}
}
