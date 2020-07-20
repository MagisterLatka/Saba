#include "pch.h"
#include "Quad.h"

#include "Renderer3D.h"
#include <glm\gtc\matrix_transform.hpp>

namespace Saba {

	uint8_t Quad::s_ModelID = 255;

	Quad::Quad(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, glm::vec4 color, bool isLighted, float shininess)
		: Scene3DObject(isLighted), m_Pos(pos), m_Size(size), m_Color(color), m_Shininess(shininess)
	{
		if (s_ModelID == 255)
			CreateModel();
		SetDirection(dir);
	}
	Quad::Quad(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, Ref<Texture2D> texture, Ref<Texture2D> specTexture, bool isLighted, float shininess)
		: Scene3DObject(isLighted), m_Pos(pos), m_Size(size), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_Shininess(shininess), m_Texture(texture), m_SpecTexture(specTexture)
	{
		if (s_ModelID == 255)
			CreateModel();
		SetDirection(dir);
	}

	void Quad::Draw()
	{
		const glm::mat4 modelMat = glm::scale(glm::translate(m_RotateFromOrigin, m_Pos) * m_Rotate, m_Size);
		Renderer3D::DrawModel(s_ModelID, modelMat, m_Color, { m_Texture }, { m_SpecTexture }, m_IsLighted, m_Shininess);
	}

	void Quad::SetPos(glm::vec3 pos)
	{
		m_Pos = pos;
	}
	void Quad::Move(glm::vec3 pos)
	{
		m_Pos += pos;
	}
	void Quad::SetDirection(glm::vec3 dir)
	{
		dir = glm::normalize(dir);
		if (dir == glm::vec3(1.0f, 0.0f, 0.0f))
			m_Rotate = glm::mat4(1.0f);
		else if (dir == glm::vec3(-1.0f, 0.0f, 0.0f))
			m_Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), {0.0f, 1.0f, 0.0f});
		else
		{
			const float angle = glm::acos(glm::dot(dir, { 1.0f, 0.0f, 0.0f }));
			const glm::vec3 axis = glm::normalize(glm::cross({ 1.0f, 0.0f, 0.0f }, dir));
			m_Rotate = glm::rotate(glm::mat4(1.0f), angle, axis);
		}
	}
	void Quad::Rotate(float angleR, glm::vec3 axis)
	{
		m_Rotate = glm::rotate(m_Rotate, angleR, axis);
	}
	void Quad::SetDirectionFromOrigin(glm::vec3 dir)
	{
		dir = glm::normalize(dir);
		if (dir == glm::vec3(1.0f, 0.0f, 0.0f))
			m_RotateFromOrigin = glm::mat4(1.0f);
		else if (dir == glm::vec3(-1.0f, 0.0f, 0.0f))
			m_RotateFromOrigin = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), {0.0f, 1.0f, 0.0f});
		else
		{
			const float angle = glm::acos(glm::dot(dir, { 1.0f, 0.0f, 0.0f }));
			const glm::vec3 axis = glm::normalize(glm::cross({ 1.0f, 0.0f, 0.0f }, dir));
			m_RotateFromOrigin = glm::rotate(glm::mat4(1.0f), angle, axis);
		}
	}
	void Quad::RotateFromOrigin(float angleR, glm::vec3 axis)
	{
		m_RotateFromOrigin = glm::rotate(m_RotateFromOrigin, angleR, axis);
	}
	void Quad::SetSize(glm::vec3 size)
	{
		m_Size = size;
	}
	void Quad::Scale(glm::vec3 scale)
	{
		m_Size *= scale;
	}

	void Quad::CreateModel()
	{
		std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec2, glm::vec4, float, float, float>> posNormalUVColorTIDSpecTIDShininess = {
			{ { 0.0f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, 0.0f, 1.0f },
			{ { 0.0f, -0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, 0.0f, 1.0f },
			{ { 0.0f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, 0.0f, 1.0f },
			{ { 0.0f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f, 0.0f, 1.0f }
		};
		std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0
		};
		Renderer3D::AddModel<Quad>(RendererAPI::RenderTopology::Triangles, posNormalUVColorTIDSpecTIDShininess, indices);
	}

}
