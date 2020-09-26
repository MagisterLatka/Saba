#include "pch.h"
#include "Saba/Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Saba {

	SceneCamera::SceneCamera(Type type)
		: m_Type(type)
	{
		if (type == Type::Orthographic) m_Fov = 10.0f;
		else if (type == Type::Perspective) m_Fov = glm::quarter_pi<float>();
		Recalculate();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		Recalculate();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		SB_CORE_ASSERT(m_Type == Type::Orthographic, "Camera is not orthographic");
		m_Fov = size;
		m_NearClip = nearClip;
		m_FarClip = farClip;
		Recalculate();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		SB_CORE_ASSERT(m_Type == Type::Perspective, "Camera is not perspective");
		m_Fov = fov;
		m_NearClip = nearClip;
		m_FarClip = farClip;
		Recalculate();
	}


	void SceneCamera::Recalculate()
	{
		if (m_Type == Type::Orthographic)
			m_ProjectionMat = glm::ortho(-m_Fov * m_AspectRatio * 0.5f, m_Fov * m_AspectRatio * 0.5f, -m_Fov * 0.5f, m_Fov * 0.5f, m_NearClip, m_FarClip);
		else
			m_ProjectionMat = glm::perspective(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
	}
}