#pragma once

#include "Saba/Core.h"
#include "Saba/Renderer/Camera/OrthographicCamera.h"
#include "Saba/Timestep.h"
#include "Saba/Events/MouseEvent.h"
#include "Saba/Events/WindowEvent.h"

namespace Saba {

	class OrthographicCameraControler
	{
	public:
		OrthographicCameraControler(float aspectRatio, bool rotationEnabled = false);
		~OrthographicCameraControler();

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void Resize(float width, float height);

		float GetWidth() const { return m_AspectRatio * 2.0f * m_Zoom; }
		float GetHeight() const { return 2.0f * m_Zoom; }

		void SetZoom(float zoom) { m_Zoom = zoom; }
		float GetZoom() const { return m_Zoom; }

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_Zoom = 1.0f;
		OrthographicCamera m_Camera;

		bool m_RotationEnabled;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraMovementSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}
