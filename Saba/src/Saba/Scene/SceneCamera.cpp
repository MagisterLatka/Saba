#include "pch.h"
#include "Saba/Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Saba {

	SceneCamera::SceneCamera()
	{
		Recalculate();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_NearClip = nearClip;
		m_FarClip = farClip;
		Recalculate();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		Recalculate();
	}

	void SceneCamera::Recalculate()
	{
		m_ProjectionMat = glm::ortho(-m_OrthographicSize * m_AspectRatio * 0.5f, m_OrthographicSize * m_AspectRatio * 0.5f, -m_OrthographicSize * 0.5f, m_OrthographicSize * 0.5f, m_NearClip, m_FarClip);
	}
}