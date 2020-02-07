#pragma once

#include <glm\glm.hpp>

namespace Saba {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformFloat1(const char* name, float value) = 0;
		virtual void SetUniformFloat1v(const char* name, float* value, int count) = 0;
		virtual void SetUniformFloat2(const char* name, const glm::vec2& value) = 0;
		virtual void SetUniformFloat3(const char* name, const glm::vec3& value) = 0;
		virtual void SetUniformFloat4(const char* name, const glm::vec4& value) = 0;

		virtual void SetUniformInt1(const char* name, int value) = 0;
		virtual void SetUniformInt1v(const char* name, int* value, int count) = 0;
		virtual void SetUniformInt2(const char* name, const glm::ivec2& value) = 0;
		virtual void SetUniformInt3(const char* name, const glm::ivec3& value) = 0;
		virtual void SetUniformInt4(const char* name, const glm::ivec4& value) = 0;

		virtual void SetUniformMat3(const char* name, const glm::mat3& value) = 0;
		virtual void SetUniformMat4(const char* name, const glm::mat4& value) = 0;

		static Ref<Shader> Create(const char* vertexSrc, const char* fragmentSrc);
	};

}
