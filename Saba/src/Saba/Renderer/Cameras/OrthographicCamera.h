#pragma once

namespace Saba {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		
		void SetProjectionMat(float left, float right, float bottom, float top);
		
		void SetPosition(const glm::vec3& pos);
		inline const glm::vec3& GetPosition() const { return m_Position; }
		void SetRotation(float rotation);
		inline float GetRotation() const { return m_Rotation; }

		inline const glm::mat4& GetProjectionMat() const { return m_ProjectionMat; }
		inline const glm::mat4& GetViewMat() const { return m_ViewMat; }
		inline const glm::mat4& GetViewProjectionMat() const { return m_ViewProjMat; }
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
