#include "pch.h"
#include "Cube.h"

#include <glm\gtc\matrix_transform.hpp>
#include "Renderer3D.h"

namespace Saba {

	uint8_t Cube::s_ModelID = 255;

	Cube::Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, glm::vec4 color)
		: m_Pos(pos), m_Size(size), m_Color(color), m_Textured(false)
	{
		if (s_ModelID == 255)
			CreateModel();

		SetDirection(dir);
	}
	Cube::Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, Ref<Texture2D> texture)
		: m_Pos(pos), m_Size(size), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_Textured(true), m_Textures({ texture, texture, texture, texture, texture, texture })
	{
		if (s_ModelID == 255)
			CreateModel();

		SetDirection(dir);
	}
	Cube::Cube(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, const std::array<Ref<Texture2D>, 6> & textures)
		: m_Pos(pos), m_Size(size), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_Textured(true), m_Textures(textures)
	{
		if (s_ModelID == 255)
			CreateModel();

		SetDirection(dir);
	}

	void Cube::Draw()
	{
		const glm::mat4 modelMat = glm::scale(glm::translate(m_RotateFromOrigin, m_Pos) * m_Rotate, m_Size);

		std::vector<Ref<Texture2D>> textures(6);
		if (m_Textured)
		{
			for (int i = 0; i < 6; i++)
				textures[i] = m_Textures[i];
		}
		Renderer3D::DrawModel(s_ModelID, modelMat, m_Color, textures);
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
	void Cube::Rotate(float angle, glm::vec3 axis)
	{
		m_Rotate = glm::rotate(m_Rotate, angle, axis);
	}

	void Cube::SetDirectionFromOrigin(glm::vec3 dir)
	{
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
	void Cube::RotateFromOrigin(float angleR, glm::vec3 axis)
	{
		m_RotateFromOrigin = glm::rotate(m_RotateFromOrigin, angleR, axis);
	}

	void Cube::SetSize(glm::vec3 size)
	{
		m_Size = size;
	}
	void Cube::Scale(glm::vec3 scale)
	{
		m_Size *= scale;
	}

	void Cube::CreateModel()
	{
		std::vector<std::tuple<glm::vec3, glm::vec2, glm::vec4, float>> posUVColorTID = {
			{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},

			{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},

			{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},

			{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},

			{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},

			{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},
			{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f}
		};
		std::vector<uint32_t> indices = {
			0,  1,  2,  2,  3,  0,
			4,  5,  6,  6,  7,  4,
			8,  9,  10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20,
		};

		Renderer3D::AddModel<Cube>(RendererAPI::Triangles, posUVColorTID, indices, {});
	}
}
