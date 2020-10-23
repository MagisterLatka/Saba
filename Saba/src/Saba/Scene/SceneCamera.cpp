#include "pch.h"
#include "Saba/Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Saba {

	SceneCamera::SceneCamera(Type type)
		: m_Type(type) {}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		Recalculate();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		Recalculate();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_PerspectiveFov = fov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		Recalculate();
	}


	void SceneCamera::Recalculate()
	{
		if (m_Type == Type::Orthographic)
			m_ProjectionMat = glm::ortho(-m_OrthographicSize * m_AspectRatio * 0.5f, m_OrthographicSize * m_AspectRatio * 0.5f, -m_OrthographicSize * 0.5f, m_OrthographicSize * 0.5f,
										 m_OrthographicNear, m_OrthographicFar);
		else
			m_ProjectionMat = glm::perspective(m_PerspectiveFov, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
	}
}