#include "pch.h"
#include "OrthographicCameraControler.h"
#include "Saba\Input.h"

#include "GLFW\glfw3.h"

namespace Saba {

	OrthographicCameraControler::OrthographicCameraControler(float aspectRatio, bool rotationEnabled)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_Zoom, m_AspectRatio* m_Zoom, -m_Zoom, m_Zoom), m_RotationEnabled(rotationEnabled)
	{
	}

	OrthographicCameraControler::~OrthographicCameraControler()
	{
	}

	void OrthographicCameraControler::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * (float)ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * (float)ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * (float)ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * (float)ts;
		}

		if (Input::IsKeyPressed(GLFW_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * (float)ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * (float)ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * (float)ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * (float)ts;
		}

		if (m_RotationEnabled)
		{
			if (Input::IsKeyPressed(GLFW_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * (float)ts;
			else if (Input::IsKeyPressed(GLFW_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * (float)ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraMovementSpeed = m_Zoom * 3.0f;
	}
	void OrthographicCameraControler::OnEvent(Event& e)
	{
		Dispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SB_BIND_EVENT_FUNC(OrthographicCameraControler::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(SB_BIND_EVENT_FUNC(OrthographicCameraControler::OnWindowResized));
	}

	bool OrthographicCameraControler::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Zoom -= e.GetYOffset() * 0.25f;
		m_Zoom = std::max(m_Zoom, 0.25f);
		m_Camera.SetProjectionMat(-m_AspectRatio * m_Zoom, m_AspectRatio* m_Zoom, -m_Zoom, m_Zoom);
		return false;
	}
	bool OrthographicCameraControler::OnWindowResized(WindowResizeEvent& e) //TODO: Viewports resize
	{
		m_AspectRatio = (float)e.GetXSize() / (float)e.GetYSize();
		m_Camera.SetProjectionMat(-m_AspectRatio * m_Zoom, m_AspectRatio* m_Zoom, -m_Zoom, m_Zoom);
		return false;
	}
}
