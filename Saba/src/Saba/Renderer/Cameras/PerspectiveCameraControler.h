#pragma once

#include "PerspectiveCamera.h"
#include "Saba\Timestep.h"
#include "Saba\Events\MouseEvent.h"
#include "Saba\Events\WindowEvent.h"

namespace Saba {

	class PerspectiveCameraControler
	{
	public:
		PerspectiveCameraControler(float aspectRatio, float radiansFOV, float nearZ, float farZ);
		~PerspectiveCameraControler() = default;

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		void SetZoom(float zoom);
		inline float GetZoom() const { return m_Zoom; }

		void SetPosition(glm::vec3 pos);
		inline glm::vec3 GetPosition() const { return m_CameraPosition; }

		void SetRotation(float yaw, float pitch);
		inline std::pair<float, float> GetRotation() const { return { m_Yaw, m_Pitch }; }
		void EnableRotation(bool enable) { m_EnableRotation = enable; }
		inline bool IsRotationEnabled() const { return m_EnableRotation; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_Zoom = 90.0f;
		float m_Near, m_Far;
		PerspectiveCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_Yaw = 0.0f, m_Pitch = 0.0f;
		float m_CameraMovementSpeed = 5.0f, m_MouseSensivity = 0.1f;
		bool m_EnableRotation = true;

		float m_LastMouseX, m_LastMouseY;
	};

}
