#include "pch.h"
#include "Sphere.h"
#include "Renderer3D.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Saba {

	Sphere::Sphere(uint8_t segments, glm::vec3 pos, glm::vec3 size, glm::vec3 dir, glm::vec4 color)
		: m_Pos(pos), m_Size(size), m_Direction(dir), m_Color(color), m_Textured(false)
	{
		CreateSphereMesh(segments);
	}
	Sphere::Sphere(uint8_t segments, glm::vec3 pos, glm::vec3 size, glm::vec3 dir, Ref<Texture2D> texture)
		: m_Pos(pos), m_Size(size), m_Direction(dir), m_Textured(true), m_Texture(texture)
	{
		CreateSphereMesh(segments);
	
	}
	void Sphere::Draw()
	{
		std::vector<std::pair<glm::vec3, glm::vec2>> posUV(m_PosUV.size());
		const float angle = glm::acos(glm::dot(m_Direction, {1.0f, 0.0f, 0.0f}));
		const glm::vec3 axis = m_Direction == glm::vec3(1.0f, 0.0f, 0.0f) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::normalize(glm::cross({ 1.0f, 0.0f, 0.0f }, m_Direction));
		const glm::mat4 modelMat = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), m_Pos), angle, axis), m_Size);
		for (int i = 0; i < (int)m_PosUV.size(); i++)
		{
			posUV[i].first = glm::vec3(modelMat * glm::vec4(m_PosUV[i].first, 1.0f));
			posUV[i].second = m_PosUV[i].second;
		}
		if (!m_Textured)
			Renderer3D::DrawTriangleStrip(posUV, m_Indices, m_Color);
		else
			Renderer3D::DrawTriangleStrip(posUV, m_Indices, m_Texture);
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
		m_Direction = dir;
	}
	void Sphere::Rotate(float angleR, glm::vec3 axis)
	{
		const glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angleR, axis);
		m_Direction = glm::mat3(rotate) * m_Direction;
	}
	void Sphere::SetSize(glm::vec3 size)
	{
		m_Size = size;
	}
	void Sphere::Scale(glm::vec3 scale)
	{
		m_Size *= scale;
	}

	void Sphere::CreateSphereMesh(uint8_t segments)
	{
		for (int y = 0; y <= segments; y++)
		{
			for (int x = 0; x <= segments; x++)
			{
				const float xSegment = (float)x / (float)segments;
				const float ySegment = (float)y / (float)segments;
				const float xPos = glm::cos(xSegment * 2.0f * glm::pi<float>()) * glm::sin(ySegment * glm::pi<float>());
				const float yPos = glm::cos(ySegment * glm::pi<float>());
				const float zPos = glm::sin(xSegment * 2.0f * glm::pi<float>()) * glm::sin(ySegment * glm::pi<float>());
				m_PosUV.push_back({ {xPos, yPos, zPos}, {xSegment, ySegment} });
			}
		}
		bool oddRow = false;
		for (int y = 0; y < segments; y++)
		{
			if (!oddRow)
			{
				for (int x = 0; x <= segments; x++)
				{
					m_Indices.push_back(y       * (segments + 1) + x);
					m_Indices.push_back((y + 1) * (segments + 1) + x);
				}
			}
			else
			{
				for (int x = segments; x >= 0; x--)
				{
					m_Indices.push_back((y + 1) * (segments + 1) + x);
					m_Indices.push_back(y       * (segments + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
	}
}
