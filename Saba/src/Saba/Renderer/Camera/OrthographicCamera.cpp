#include "pch.h"
#include "Saba/Renderer/Camera/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Saba {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMat(1.0f)
	{
		Recalculate();
	}

	void OrthographicCamera::SetProjectionMat(float left, float right, float bottom, float top)
	{
		m_ProjectionMat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		Recalculate();
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		m_ViewMat = glm::rotate(glm::translate(glm::mat4(1.0f), -m_Position), glm::radians(-m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		Recalculate();
	}
	void OrthographicCamera::SetRotation(float rotation)
	{
		m_Rotation = rotation;
		m_ViewMat = glm::rotate(glm::translate(glm::mat4(1.0f), -m_Position), glm::radians(-m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		Recalculate();
	}

	void OrthographicCamera::Recalculate()
	{
		m_ViewProjMat = m_ProjectionMat * m_ViewMat;
	}

}
