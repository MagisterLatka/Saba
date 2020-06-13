#pragma once

#include "Scene3DObject.h"
#include "Saba\Renderer\Texture.h"

namespace Saba {

	class Cube : public Scene3DObject
	{
		friend class Renderer3D;
	public:
		//pos of the cube, size of the cube, dir of X+ side and color of the cube
		Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, bool isLighted = true);
		//pos of the cube, size of the cube, dir of X+ side and texture - same for all sides
		Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, Ref<Texture2D> texture, bool isLighted = true);
		~Cube() = default;

		virtual void Draw() override;
		virtual void SetPos(glm::vec3 pos) override;
		virtual void Move(glm::vec3 pos) override;
		virtual void SetDirection(glm::vec3 dir) override;
		virtual void Rotate(float angleR, glm::vec3 axis) override;
		virtual void SetDirectionFromOrigin(glm::vec3 dir) override;
		virtual void RotateFromOrigin(float angleR, glm::vec3 axis) override;
		virtual void SetSize(glm::vec3 size) override;
		virtual void Scale(glm::vec3 scale) override;
		inline static uint8_t GetModelID() { return s_ModelID; }
	private:
		static void SetModelID(uint8_t id) { s_ModelID = id; }
		static void CreateModel();
	private:
		static uint8_t s_ModelID;
		glm::vec3 m_Pos;
		glm::vec3 m_Size;
		glm::mat4 m_Rotate = glm::mat4(1.0f);
		glm::mat4 m_RotateFromOrigin = glm::mat4(1.0f);
		glm::vec4 m_Color;
		Ref<Texture2D> m_Texture;
	};
}
