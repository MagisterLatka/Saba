#include "pch.h"
#include "PerspectiveCamera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Saba {

	PerspectiveCamera::PerspectiveCamera(float aspectRatio, float radiansFOV, float nearZ, float farZ, glm::vec3 worldUp)
		:m_ProjMat(glm::perspective(radiansFOV, aspectRatio, nearZ, farZ)), m_WorldUp(worldUp)
	{
		Recaulculate();
	}

	void PerspectiveCamera::SetProjectionMat(float aspectRatio, float radiansFOV, float nearZ, float farZ)
	{
		m_ProjMat = glm::perspective(radiansFOV, aspectRatio, nearZ, farZ);
		m_ViewProjMat = m_ProjMat * m_ViewMat;
	}

	void PerspectiveCamera::SetPosition(glm::vec3 position)
	{
		m_Position = position;
		Recaulculate();
	}

	void PerspectiveCamera::SetRotation(float yaw, float pitch)
	{
		m_Yaw = yaw;
		m_Pitch = pitch;

		if (m_Yaw <= -180.0f) m_Yaw += 360.0f;
		if (m_Yaw >=  180.0f) m_Yaw -= 360.0f;
		if (m_Pitch < -89.9f) m_Pitch = -89.9f;
		if (m_Pitch >  89.9f) m_Pitch =  89.9f;

		m_Direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
		m_Direction.y = glm::sin(glm::radians(pitch));
		m_Direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
		CalculateVectors();
		Recaulculate();
	}

	void PerspectiveCamera::SetRotation(glm::vec3 dir)
	{
		m_Direction = glm::normalize(dir);
		m_Pitch = glm::degrees(glm::acos(dir.y));
		m_Yaw = glm::degrees(glm::acos(m_Direction.z / glm::cos(glm::radians(m_Pitch))));
		CalculateVectors();
		Recaulculate();
	}

	void PerspectiveCamera::Recaulculate()
	{
		m_ViewMat = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
		m_ViewProjMat = m_ProjMat * m_ViewMat;
	}

	void PerspectiveCamera::CalculateVectors()
	{
		glm::vec3 right = glm::normalize(glm::cross(m_Direction, m_WorldUp));
		m_Up = glm::normalize(glm::cross(right, m_Direction));
	}

}
