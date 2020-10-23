#pragma once

#include "Saba/Core.h"
#include "Saba/Renderer/Camera.h"

namespace Saba {

	class SceneCamera : public Camera
	{
		friend class SceneHierarchyPanel;
	public:
		enum class Type { Perspective = 0, Orthographic = 1};
	public:
		SceneCamera() = default;
		SceneCamera(Type type);
		virtual ~SceneCamera() = default;

		void SetViewportSize(uint32_t width, uint32_t height);

		Type GetType() const { return m_Type; }
		void SetType(Type type) { m_Type = type; }

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetOrthographicSize(float size) { m_OrthographicSize = size; Recalculate(); }
		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; Recalculate(); }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; Recalculate(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		float GetOrthographicWidth() const { return m_OrthographicSize * m_AspectRatio; }
		float GetOrthographicHeight() const { return m_OrthographicSize; }

		void SetPerspective(float fov, float nearClip, float farClip);
		void SetPerspectiveFOV(float fov) { m_PerspectiveFov = fov; Recalculate(); }
		float GetPerspectiveFOV() const { return m_PerspectiveFov; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; Recalculate(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; Recalculate(); }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
	private:
		void Recalculate();
	private:
		Type m_Type = Type::Orthographic;

		float m_PerspectiveFov = 1.570796371f; //error with glm::half_pi<float>(), lol
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 100.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};
}
