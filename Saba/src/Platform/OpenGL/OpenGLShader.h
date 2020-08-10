#pragma once

#include "Saba/Renderer/Shader.h"
#include <glad/glad.h>

namespace Saba {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniformFloat1(const std::string& name, float value) override;
		virtual void SetUniformFloat1v(const std::string& name, float* value, int count) override;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;

		virtual void SetUniformInt1(const std::string& name, int value) override;
		virtual void SetUniformInt1v(const std::string& name, int* value, int count) override;
		virtual void SetUniformInt2(const std::string& name, const glm::ivec2& value) override;
		virtual void SetUniformInt3(const std::string& name, const glm::ivec3& value) override;
		virtual void SetUniformInt4(const std::string& name, const glm::ivec4& value) override;

		virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) override;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) override;
	private:
		std::string Read(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& sources);

		int GetUniformLocation(const std::string& name);
	private:
		unsigned int m_ID;
		std::unordered_map<std::string, int> m_UniformLocations;
	};

}
