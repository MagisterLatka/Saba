#pragma once

#include "Saba/Renderer/Camera.h"

namespace Saba {

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; Recalculate(); }
		float GetWidth() const { return m_OrthographicSize * m_AspectRatio; }
		float GetHeight() const { return m_OrthographicSize; }
	private:
		void Recalculate();
	private:
		float m_OrthographicSize = 10.0f;
		float m_NearClip = -1.0f, m_FarClip = 1.0f;
		float m_AspectRatio = 0.0f;
	};
}
