#pragma once

#include "Scene3DObject.h"
#include "Saba\Renderer\Texture.h"

namespace Saba {

	class Cube : public Scene3DObject
	{
	public:
		//pos of the cube, size of the cube, dir of X+ side and color of the cube
		Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//pos of the cube, size of the cube, dir of X+ side and texture - same for all sides
		Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, Ref<Texture2D> texture);
		//pos of the cube, size of the cube, dir of X+ side and textures (0 for X+, 1 for X-, 2 for Y+, 3 for Y-, 4 for Z+, 5 for Z-)
		Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, const std::array<Ref<Texture2D>, 6>& textures);
		~Cube() = default;

		virtual void Draw() override;
		virtual void SetPos(glm::vec3 pos) override;
		virtual void Move(glm::vec3 pos) override;
		virtual void SetDirection(glm::vec3 dir) override;
		virtual void Rotate(float angleR, glm::vec3 axis) override;
		virtual void SetSize(glm::vec3 size) override;
		virtual void Scale(glm::vec3 scale) override;
	private:
		glm::vec3 m_Pos;
		glm::vec3 m_Size;
		glm::vec3 m_Direction;
		glm::vec4 m_Color;
		const bool m_Textured;
		std::array<Ref<Texture2D>, 6> m_Textures;
	};
}
