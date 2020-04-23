#pragma once

#include "Scene3DObject.h"
#include "Saba\Renderer\Texture.h"

namespace Saba {

	class Sphere : public Scene3DObject
	{
		friend class Renderer3D;
	public:
		Sphere(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, glm::vec4 color);
		Sphere(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, Ref<Texture2D> texture);
		~Sphere() = default;

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
		static void CreateSphereMesh();
	private:
		static uint8_t s_ModelID;
		static std::vector<std::tuple<glm::vec3, glm::vec2, glm::vec4, float>> s_PosUVColorTID;
		static std::vector<uint32_t> s_Indices;
		glm::vec3 m_Pos;
		glm::vec3 m_Size;
		glm::mat4 m_Rotate = glm::mat4(1.0f);
		glm::mat4 m_RotateFromOrigin = glm::mat4(1.0f);
		glm::vec4 m_Color;
		Ref<Texture2D> m_Texture;
	};
}
