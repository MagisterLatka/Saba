#pragma once

#include "Scene3DObject.h"
#include "Saba\Renderer\Texture.h"

namespace Saba {

	class Sphere : public Scene3DObject
	{
	public:
		Sphere(uint8_t segments, glm::vec3 pos, glm::vec3 size, glm::vec3 dir, glm::vec4 color);
		Sphere(uint8_t segments, glm::vec3 pos, glm::vec3 size, glm::vec3 dir, Ref<Texture2D> texture);
		~Sphere() = default;

		virtual void Draw() override;
		virtual void SetPos(glm::vec3 pos) override;
		virtual void Move(glm::vec3 pos) override;
		virtual void SetDirection(glm::vec3 dir) override;
		virtual void Rotate(float angleR, glm::vec3 axis) override;
		virtual void SetSize(glm::vec3 size) override;
		virtual void Scale(glm::vec3 scale) override;
	private:
		void CreateSphereMesh(uint8_t segments);
	private:
		std::vector<std::pair<glm::vec3, glm::vec2>> m_PosUV;
		std::vector<uint32_t> m_Indices;
		glm::vec3 m_Pos;
		glm::vec3 m_Size;
		glm::vec3 m_Direction;
		glm::vec4 m_Color;
		const bool m_Textured;
		Ref<Texture2D> m_Texture;
	};
}
