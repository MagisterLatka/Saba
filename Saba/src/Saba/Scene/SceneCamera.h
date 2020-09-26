#pragma once

#include "Saba/Core.h"
#include "Saba/Renderer/Camera.h"

namespace Saba {

	class SceneCamera : public Camera
	{
		friend class SceneHierarchyPanel;
	public:
		enum class Type { Orthographic, Perspective };
	public:
		SceneCamera(Type type);
		virtual ~SceneCamera() = default;

		void SetViewportSize(uint32_t width, uint32_t height);

		Type GetType() const { return m_Type; }

		void SetOrthographic(float size, float nearClip, float farClip);
		float GetOrthographicSize() const { SB_CORE_ASSERT(m_Type == Type::Orthographic, "Camera is not orthographic"); return m_Fov; }
		void SetOrthographicSize(float size) { SB_CORE_ASSERT(m_Type == Type::Orthographic, "Camera is not orthographic"); m_Fov = size; Recalculate(); }
		float GetWidth() const { return m_Fov * m_AspectRatio; }
		float GetHeight() const { return m_Fov; }

		void SetPerspective(float fov, float nearClip, float farClip);
		void SetFov(float fov) { SB_CORE_ASSERT(m_Type == Type::Perspective, "Camera is not perspective"); m_Fov = fov; }
		float GetFov() const { SB_CORE_ASSERT(m_Type == Type::Perspective, "Camera is not perspective"); return m_Fov; }

		float GetNearClip() const { return m_NearClip; }
		float GetFarClip() const { return m_FarClip; }
		float GetAspectRatio() const { return m_AspectRatio; }
	private:
		void Recalculate();
	private:
		Type m_Type;

		float m_Fov = 0.0f;

		float m_NearClip = -1.0f, m_FarClip = 1.0f;
		float m_AspectRatio = 0.0f;
	};
}
