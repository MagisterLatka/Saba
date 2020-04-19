#include "pch.h"
#include "PerspectiveCameraControler.h"
#include "Saba\Input.h"

#include "GLFW\glfw3.h"

namespace Saba {

	PerspectiveCameraControler::PerspectiveCameraControler(float aspectRatio, float radiansFOV, float nearZ, float farZ)
		: m_Camera(aspectRatio, radiansFOV, nearZ, farZ), m_AspectRatio(aspectRatio), m_Near(nearZ), m_Far(farZ)
	{
		m_LastMouseX = Input::GetMouseXPos();
		m_LastMouseY = Input::GetMouseYPos();
	}

	void PerspectiveCameraControler::SetZoom(float zoom) 
	{
		m_Zoom = zoom;
	}

	void PerspectiveCameraControler::SetPosition(glm::vec3 pos)
	{
		m_CameraPosition = pos;
	}

	void PerspectiveCameraControler::SetRotation(float yaw, float pitch)
	{
		m_Yaw = yaw;
		m_Pitch = pitch;
	}

	void PerspectiveCameraControler::SetMovementSpeed(float speed)
	{
		m_CameraMovementSpeed = speed;
	}

	void PerspectiveCameraControler::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(GLFW_KEY_W))
		{
			m_CameraPosition += glm::vec3(m_Camera.GetDirection().x, 0.0f, m_Camera.GetDirection().z) * m_CameraMovementSpeed * (float)ts;
		}
		if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_CameraPosition -= glm::vec3(m_Camera.GetDirection().x, 0.0f, m_Camera.GetDirection().z) * m_CameraMovementSpeed * (float)ts;
		}
		if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			glm::vec3 right = glm::normalize(glm::cross(m_Camera.GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
			m_CameraPosition += right * m_CameraMovementSpeed * (float)ts;
		}
		if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			glm::vec3 right = glm::normalize(glm::cross(m_Camera.GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
			m_CameraPosition -= right * m_CameraMovementSpeed * (float)ts;
		}
		if (Input::IsKeyPressed(GLFW_KEY_SPACE))
		{
			m_CameraPosition += glm::vec3(0.0f, 1.0f, 0.0f) * m_CameraMovementSpeed * (float)ts;
		}
		if (Input::IsKeyPressed(GLFW_KEY_C))
		{
			m_CameraPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * m_CameraMovementSpeed * (float)ts;
		}
		m_Camera.SetProjectionMat(m_AspectRatio, glm::radians(m_Zoom), m_Near, m_Far);
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_Yaw, m_Pitch);
	}

	void PerspectiveCameraControler::OnEvent(Event& e)
	{
		Dispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SB_BIND_EVENT_FUNC(PerspectiveCameraControler::OnMouseScrolled));
		dispatcher.Dispatch<MouseMovedEvent>(SB_BIND_EVENT_FUNC(PerspectiveCameraControler::OnMouseMoved));
		dispatcher.Dispatch<WindowResizeEvent>(SB_BIND_EVENT_FUNC(PerspectiveCameraControler::OnWindowResized));
	}

	bool PerspectiveCameraControler::OnMouseMoved(MouseMovedEvent& e)
	{
		if (m_EnableRotation)
		{
			//float xoffset = m_LastMouseX - e.GetXPos();
			float xoffset = e.GetXPos() - m_LastMouseX;
			float yoffset = m_LastMouseY - e.GetYPos();

			m_Yaw += xoffset * m_MouseSensivity;
			m_Pitch += yoffset * m_MouseSensivity;

			if (m_Yaw >=  180.0f) m_Yaw -= 360.0f;
			if (m_Yaw <= -180.0f) m_Yaw += 360.0f;
			if (m_Pitch >  89.9f) m_Pitch =  89.9f;
			if (m_Pitch < -89.9f) m_Pitch = -89.9f;
		}
		m_LastMouseX = e.GetXPos();
		m_LastMouseY = e.GetYPos();
		return false;
	}

	bool PerspectiveCameraControler::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Zoom -= e.GetYOffset();
		if (m_Zoom > 90.0f) m_Zoom = 90.0f;
		if (m_Zoom < 1.0f)  m_Zoom = 1.0f;
		return false;
	}

	bool PerspectiveCameraControler::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetYSize() / (float)e.GetXSize();
		return false;
	}

}
