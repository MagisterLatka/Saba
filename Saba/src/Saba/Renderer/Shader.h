#pragma once

#include "Saba/Core.h"
#include <glm/glm.hpp>

namespace Saba {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Recompile(const std::string& vertexSrc, const std::string& fragmentSrc) = 0;
		virtual void Recompile(const std::string& vertexSrc, const std::string& geometrySrc, const std::string& fragmentSrc) = 0;
		virtual void Recompile(const std::string& filepath) = 0;

		virtual void SetUniformFloat1(const std::string& name, float value) = 0;
		virtual void SetUniformFloat1v(const std::string& name, float* value, int count) = 0;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void SetUniformInt1(const std::string& name, int value) = 0;
		virtual void SetUniformInt1v(const std::string& name, int* value, int count) = 0;
		virtual void SetUniformInt2(const std::string& name, const glm::ivec2& value) = 0;
		virtual void SetUniformInt3(const std::string& name, const glm::ivec3& value) = 0;
		virtual void SetUniformInt4(const std::string& name, const glm::ivec4& value) = 0;

		virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual bool HasUniform(const std::string& name) = 0;

		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& geometrySrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& filepath);
	};

}
