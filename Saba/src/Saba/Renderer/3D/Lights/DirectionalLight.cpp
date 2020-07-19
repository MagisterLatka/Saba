#include "pch.h"
#include "DirectionalLight.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Saba {

	DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 diffuseColor, glm::vec3 specularColor)
		: Light(-1)
	{
		m_Dir = glm::normalize(dir);
		m_DiffuseColor = diffuseColor;
		m_SpecularColor = specularColor;
		m_AttConstant = 1.0f;
		m_AttLinear = 0.0f;
		m_AttQuadratic = 0.0f;

		m_LightSpace = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, c_NearPlane, c_FarPlane) *
		glm::lookAt(-m_Dir * 0.5f * c_FarPlane, { 0.0f, 0.0f, 0.0f },
					glm::normalize(glm::cross(m_Dir,
					m_Dir == glm::vec3(0.0f, 1.0f, 0.0f) || m_Dir == glm::vec3(0.0f, -1.0f, 0.0f) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Dir)))));
	}

	void DirectionalLight::SetDir(glm::vec3 dir)
	{
		m_Dir = glm::normalize(dir);

		m_LightSpace = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, c_NearPlane, c_FarPlane) *
		glm::lookAt(-m_Dir * 0.5f * c_FarPlane, { 0.0f, 0.0f, 0.0f },
					glm::normalize(glm::cross(m_Dir,
					m_Dir == glm::vec3(0.0f, 1.0f, 0.0f) || m_Dir == glm::vec3(0.0f, -1.0f, 0.0f) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Dir)))));
	}
	void DirectionalLight::SetDiffuseColor(glm::vec3 color)
	{
		m_DiffuseColor = color;
	}
	void DirectionalLight::SetSpecularColor(glm::vec3 color)
	{
		m_SpecularColor = color;
	}

	const glm::mat4* DirectionalLight::SetShadowData(const std::pair<glm::vec2, glm::vec2>& shadowTextureSpace)
	{
		m_ShadowTextureSpace = shadowTextureSpace;

		return &m_LightSpace;
	}
	Light::LightData* DirectionalLight::GetData()
	{
		LightData* data = new LightData;
		data->pos.w = 0.0f;
		data->dir = glm::vec4(m_Dir, 1.0f);
		data->diffuseColor = glm::vec4(m_DiffuseColor, 0.0f);
		data->specularColor = glm::vec4(m_SpecularColor, 0.0f);
		data->cutsoff_FarPlane = glm::vec4(0.0f);
		data->attenuation = glm::vec4(m_AttConstant, m_AttLinear, m_AttQuadratic, 0.0f);
		data->shadowTextureSpace = glm::vec4(m_ShadowTextureSpace.first, m_ShadowTextureSpace.second);
		data->lightSpace = m_LightSpace;
		return data;
	}
	Light::LightData* DirectionalLight::GetData(LightData* bufferPtr)
	{
		bufferPtr->pos.w = 0.0f;
		bufferPtr->dir = glm::vec4(m_Dir, 1.0f);
		bufferPtr->diffuseColor = glm::vec4(m_DiffuseColor, 0.0f);
		bufferPtr->specularColor = glm::vec4(m_SpecularColor, 0.0f);
		bufferPtr->cutsoff_FarPlane = glm::vec4(0.0f);
		bufferPtr->attenuation = glm::vec4(m_AttConstant, m_AttLinear, m_AttQuadratic, 0.0f);
		bufferPtr->shadowTextureSpace = glm::vec4(m_ShadowTextureSpace.first, m_ShadowTextureSpace.second);
		bufferPtr->lightSpace = m_LightSpace;
		return bufferPtr;
	}
}
