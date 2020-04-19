#pragma once

namespace Saba {

	class Scene3DObject
	{
		friend class Scene3D;
	public:
		Scene3DObject() = default;
		virtual ~Scene3DObject() = default;

		inline uint32_t GetID() const { return m_ID; }
		virtual void Draw() = 0;
		virtual void SetPos(glm::vec3 pos) = 0;
		virtual void Move(glm::vec3 pos) = 0;
		virtual void SetDirection(glm::vec3 dir) = 0;
		virtual void Rotate(float angleR, glm::vec3 axis) = 0;
		virtual void SetSize(glm::vec3 size) = 0;
		virtual void Scale(glm::vec3 scale) = 0;
	private:
		uint32_t m_ID = -1;
	};
}
