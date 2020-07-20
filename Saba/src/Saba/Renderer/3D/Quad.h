#pragma once

#include "Scene3DObject.h"
#include "Saba\Renderer\Texture.h"

namespace Saba {

	class Quad : public Scene3DObject
	{
		friend class Renderer3D;
	public:
		Quad(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, glm::vec4 color, bool isLighted = true, float shininess = 1.0f);
		Quad(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, Ref<Texture2D> texture, Ref<Texture2D> specTexture = {}, bool isLighted = true, float shininess = 1.0f);
		~Quad() = default;

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
		static void CreateModel();
		static void SetModelID(uint8_t id) { s_ModelID = id; }
	private:
		static uint8_t s_ModelID;
		glm::vec3 m_Pos;
		glm::vec3 m_Size;
		glm::mat4 m_Rotate = glm::mat4(1.0f);
		glm::mat4 m_RotateFromOrigin = glm::mat4(1.0f);
		glm::vec4 m_Color;
		float m_Shininess;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpecTexture;
	};
}
