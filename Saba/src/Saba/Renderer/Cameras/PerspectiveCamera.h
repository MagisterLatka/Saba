#pragma once

namespace Saba {

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float aspectRatio, float radiansFOV, float nearZ, float farZ, glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f });

		void SetProjectionMat(float aspectRatio, float radiansFOV, float nearZ, float farZ);

		void SetPosition(glm::vec3 position);
		inline glm::vec3 GetPosition() const { return m_Position; }
		void SetRotation(float yaw, float pitch);
		inline std::pair<float, float> GetRotation() const { return { m_Yaw, m_Pitch }; }
		void SetRotation(glm::vec3 dir);
		inline glm::vec3 GetDirection() const { return m_Direction; }

		inline const glm::mat4 GetProjectionMatrix() const { return m_ProjMat; }
		inline const glm::mat4 GetViewMatrix() const { return m_ViewMat; }
		inline const glm::mat4 GetProjectionViewMatrix() const { return m_ViewProjMat; }
	private:
		void Recaulculate();
		void CalculateVectors();
	private:
		glm::mat4 m_ProjMat;
		glm::mat4 m_ViewMat;
		glm::mat4 m_ViewProjMat;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Yaw = 0.0f, m_Pitch = 0.0f;
		glm::vec3 m_Direction;
		glm::vec3 m_Up;
		const glm::vec3 m_WorldUp;
	};
}
