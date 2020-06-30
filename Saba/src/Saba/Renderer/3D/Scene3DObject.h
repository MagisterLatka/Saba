#pragma once

namespace Saba {

	class Scene3DObject
	{
		friend class Scene3D;
		friend class Renderer3D;
		friend class Light;
	public:
		Scene3DObject(bool isLighted)
			: m_IsLighted(isLighted)
		{}
		virtual ~Scene3DObject() = default;

		inline uint32_t GetID() const { return m_ID; }
		virtual void Draw() = 0;
		virtual void SetPos(glm::vec3 pos) = 0;
		virtual void Move(glm::vec3 pos) = 0;
		virtual void SetDirection(glm::vec3 dir) = 0;
		virtual void Rotate(float angleR, glm::vec3 axis) = 0;
		virtual void SetDirectionFromOrigin(glm::vec3 dir) = 0;
		virtual void RotateFromOrigin(float angleR, glm::vec3 axis) = 0;
		virtual void SetSize(glm::vec3 size) = 0;
		virtual void Scale(glm::vec3 scale) = 0;
	protected:
		bool m_IsLighted;
	private:
		uint32_t m_ID = -1;
	};
}
