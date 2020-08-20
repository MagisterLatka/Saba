#pragma once

namespace Saba {

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_ProjectionMat(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_ProjectionMat; }
	protected:
		glm::mat4 m_ProjectionMat = glm::mat4(1.0f);
	};
}
