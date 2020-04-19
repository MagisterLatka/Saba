#include "pch.h"
#include "Cube.h"

#include <glm\gtc\matrix_transform.hpp>
#include "Renderer3D.h"

namespace Saba {

	Cube::Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, glm::vec4 color)
		: m_Pos(pos), m_Size(size), m_Direction(dir), m_Color(color), m_Textured(false)
	{
	}
	Cube::Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, Ref<Texture2D> texture)
		: m_Pos(pos), m_Size(size), m_Direction(dir), m_Textured(true), m_Textures({ texture, texture, texture, texture, texture, texture })
	{
	}
	Cube::Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, const std::array<Ref<Texture2D>, 6> & textures)
		: m_Pos(pos), m_Size(size), m_Direction(dir), m_Textured(true), m_Textures(textures)
	{
	}

	void Cube::Draw()
	{
		constexpr glm::vec3 basePos[24] = {
			{ 0.5f, -0.5f,  0.5f},
			{ 0.5f, -0.5f, -0.5f},
			{ 0.5f,  0.5f, -0.5f},
			{ 0.5f,  0.5f,  0.5f},

			{-0.5f, -0.5f, -0.5f},
			{-0.5f, -0.5f,  0.5f},
			{-0.5f,  0.5f,  0.5f},
			{-0.5f,  0.5f, -0.5f},

			{-0.5f,  0.5f,  0.5f},
			{ 0.5f,  0.5f,  0.5f},
			{ 0.5f,  0.5f, -0.5f},
			{-0.5f,  0.5f, -0.5f},

			{ 0.5f, -0.5f,  0.5f},
			{ 0.5f, -0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, -0.5f,  0.5f},

			{-0.5f, -0.5f,  0.5f},
			{ 0.5f, -0.5f,  0.5f},
			{ 0.5f,  0.5f,  0.5f},
			{-0.5f,  0.5f,  0.5f},

			{ 0.5f, -0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f,  0.5f, -0.5f},
			{ 0.5f,  0.5f, -0.5f}
		};
		glm::vec3 pos[24] = {};
		for (int i = 0; i < 24; i++)
		{
			const float angle = glm::acos(glm::dot(m_Direction, {1.0f, 0.0f, 0.0f}));
			const glm::vec3 axis = m_Direction == glm::vec3(1.0f, 0.0f, 0.0f) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::normalize(glm::cross({ 1.0f, 0.0f, 0.0f }, m_Direction));
			const glm::mat4 modelMat = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), m_Pos), angle, axis), m_Size);
			pos[i] = glm::vec3(modelMat * glm::vec4(basePos[i], 1.0f));
		}
		for (int i = 0; i < 24; i += 4)
		{
			if (!m_Textured)
				Renderer3D::DrawQuad({ pos[i], pos[i + 1], pos[i + 2], pos[i + 3] }, m_Color);
			else
				Renderer3D::DrawQuad({ pos[i], pos[i + 1], pos[i + 2], pos[i + 3] }, m_Textures[i / 4]);
		}
	}

	void Cube::SetPos(glm::vec3 pos)
	{
		m_Pos = pos;
	}
	void Cube::Move(glm::vec3 pos)
	{
		m_Pos += pos;
	}

	void Cube::SetDirection(glm::vec3 dir)
	{
		m_Direction = glm::normalize(dir);
	}
	void Cube::Rotate(float angle, glm::vec3 axis)
	{
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle, axis);
		m_Direction = glm::mat3(rotate) * m_Direction;
	}

	void Cube::SetSize(glm::vec3 size)
	{
		m_Size = size;
	}
	void Cube::Scale(glm::vec3 scale)
	{
		m_Size *= scale;
	}
}
