#include "pch.h"
#include "DirectionalLight.h"

namespace Saba {

	DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 diffuseColor, glm::vec3 specularColor)
	{
		m_Pos = { 0.0f, 0.0f, 0.0f };
		m_Dir = dir;
		m_DiffuseColor = diffuseColor;
		m_SpecularColor = specularColor;
		m_AttConstant = 1.0f;
		m_AttLinear = 0.0f;
		m_AttQuadratic = 0.0f;
	}

	void DirectionalLight::SetDir(glm::vec3 dir)
	{
		m_Dir = dir;
	}
	void DirectionalLight::SetDiffuseColor(glm::vec3 color)
	{
		m_DiffuseColor = color;
	}
	void DirectionalLight::SetSpecularColor(glm::vec3 color)
	{
		m_SpecularColor = color;
	}
	Light::LightData* DirectionalLight::GetData()
	{
		LightData* data = new LightData;
		data->pos.w = 0.0f;
		data->dir = glm::vec4(m_Dir, 1.0f);
		data->diffuseColor = m_DiffuseColor;
		data->specularColor = m_SpecularColor;
		data->cutsoff = glm::vec2(0.0f);
		data->attenuation = glm::vec3(m_AttConstant, m_AttLinear, m_AttQuadratic);
		return data;
	}
	Light::LightData* DirectionalLight::GetData(LightData* bufferPtr)
	{
		bufferPtr->pos.w = 0.0f;
		bufferPtr->dir = glm::vec4(m_Dir, 1.0f);
		bufferPtr->diffuseColor = m_DiffuseColor;
		bufferPtr->specularColor = m_SpecularColor;
		bufferPtr->cutsoff = glm::vec2(0.0f);
		bufferPtr->attenuation = glm::vec3(m_AttConstant, m_AttLinear, m_AttQuadratic);
		return bufferPtr;
	}
}
