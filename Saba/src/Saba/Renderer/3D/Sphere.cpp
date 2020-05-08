#include "pch.h"
#include "Sphere.h"
#include "Renderer3D.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Saba {

	std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec2, glm::vec4, float>> Sphere::s_PosNormalUVColorTID;
	std::vector<uint32_t> Sphere::s_Indices;
	uint8_t Sphere::s_ModelID = 255;

	constexpr uint8_t c_Segments = 32;

	Sphere::Sphere(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, glm::vec4 color)
		: m_Pos(pos), m_Size(size), m_Color(color)
	{
		if (s_ModelID == 255)
		{
			CreateSphereMesh();
			Renderer3D::AddModel<Sphere>(RendererAPI::TriangleStrip, s_PosNormalUVColorTID, s_Indices, {});
		}
		SetDirection(dir);
	}
	Sphere::Sphere(glm::vec3 pos, glm::vec3 size, glm::vec3 dir, Ref<Texture2D> texture)
		: m_Pos(pos), m_Size(size), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_Texture(texture)
	{
		if (s_ModelID == 255)
		{
			CreateSphereMesh();
			Renderer3D::AddModel<Sphere>(RendererAPI::TriangleStrip, s_PosNormalUVColorTID, s_Indices, {});
		}
		SetDirection(dir);
	}
	void Sphere::Draw()
	{
		const glm::mat4 modelMat = glm::scale(glm::translate(m_RotateFromOrigin, m_Pos) * m_Rotate, m_Size);
		Renderer3D::DrawModel(s_ModelID, modelMat, m_Color, { m_Texture });
	}

	void Sphere::SetPos(glm::vec3 pos)
	{
		m_Pos = pos;
	}
	void Sphere::Move(glm::vec3 pos)
	{
		m_Pos += pos;
	}
	void Sphere::SetDirection(glm::vec3 dir)
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
	void Sphere::Rotate(float angleR, glm::vec3 axis)
	{
		m_Rotate = glm::rotate(m_Rotate, angleR, axis);
	}

	void Sphere::SetDirectionFromOrigin(glm::vec3 dir)
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
	void Sphere::RotateFromOrigin(float angleR, glm::vec3 axis)
	{
		m_RotateFromOrigin = glm::rotate(m_RotateFromOrigin, angleR, axis);
	}

	void Sphere::SetSize(glm::vec3 size)
	{
		m_Size = size;
	}
	void Sphere::Scale(glm::vec3 scale)
	{
		m_Size *= scale;
	}

	void Sphere::CreateSphereMesh()
	{
		constexpr float radius = 1.0f;
		for (uint8_t y = 0; y <= c_Segments; y++)
		{
			for (uint8_t x = 0; x <= c_Segments; x++)
			{
				const float xSegment = (float)x / (float)c_Segments;
				const float ySegment = (float)y / (float)c_Segments;
				const float xPos = glm::cos(xSegment * 2.0f * glm::pi<float>()) * glm::sin(ySegment * glm::pi<float>());
				const float yPos = glm::cos(ySegment * glm::pi<float>());
				const float zPos = glm::sin(xSegment * 2.0f * glm::pi<float>()) * glm::sin(ySegment * glm::pi<float>());

				s_PosNormalUVColorTID.push_back({
					{xPos, yPos, zPos},
					{xPos, yPos, zPos},
					{xSegment, ySegment},
					{1.0f, 1.0f, 1.0f, 1.0f},
					0.0f
				});
			}
		}

		bool oddRow = false;
		for (int y = 0; y < c_Segments; y++)
		{
			if (!oddRow)
			{
				for (int x = 0; x <= c_Segments; x++)
				{
					s_Indices.push_back(y       * (c_Segments + 1) + x);
					s_Indices.push_back((y + 1) * (c_Segments + 1) + x);
				}
			}
			else
			{
				for (int x = c_Segments; x >= 0; x--)
				{
					s_Indices.push_back((y + 1) * (c_Segments + 1) + x);
					s_Indices.push_back(y       * (c_Segments + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
	}
}
