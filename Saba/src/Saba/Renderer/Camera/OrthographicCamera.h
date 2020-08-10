#pragma once

#include <glm/glm.hpp>

namespace Saba {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		
		void SetProjectionMat(float left, float right, float bottom, float top);
		
		void SetPosition(const glm::vec3& pos);
		const glm::vec3& GetPosition() const { return m_Position; }

		void SetRotation(float rotation);
		float GetRotation() const { return m_Rotation; }

		const glm::mat4& GetProjectionMat() const { return m_ProjectionMat; }
		const glm::mat4& GetViewMat() const { return m_ViewMat; }
		const glm::mat4& GetViewProjectionMat() const { return m_ViewProjMat; }
	private:
		void Recalculate();
	private:
		glm::mat4 m_ProjectionMat;
		glm::mat4 m_ViewMat;
		glm::mat4 m_ViewProjMat;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};

}
