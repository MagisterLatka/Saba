#pragma once

#include "Saba\Renderer\Shader.h"

namespace Saba {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexSrc, const char* fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniformFloat1(const char* name, float value) override;
		virtual void SetUniformFloat1v(const char* name, float* value, int count) override;
		virtual void SetUniformFloat2(const char* name, const glm::vec2& value) override;
		virtual void SetUniformFloat3(const char* name, const glm::vec3& value) override;
		virtual void SetUniformFloat4(const char* name, const glm::vec4& value) override;

		virtual void SetUniformInt1(const char* name, int value) override;
		virtual void SetUniformInt1v(const char* name, int* value, int count) override;
		virtual void SetUniformInt2(const char* name, const glm::ivec2& value) override;
		virtual void SetUniformInt3(const char* name, const glm::ivec3& value) override;
		virtual void SetUniformInt4(const char* name, const glm::ivec4& value) override;

		virtual void SetUniformMat3(const char* name, const glm::mat3& value) override;
		virtual void SetUniformMat4(const char* name, const glm::mat4& value) override;
	private:
		void Init(const char* vertexSrc, const char* fragmentSrc);
		int GetUniformLocation(const char* name);
	private:
		uint m_ID;
		std::unordered_map<std::string_view, int> m_UniformLocations;
	};

}
