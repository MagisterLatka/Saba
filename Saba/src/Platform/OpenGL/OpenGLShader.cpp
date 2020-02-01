#include "pch.h"
#include "OpenGLShader.h"

#include <glm\gtc\type_ptr.hpp>
#include <glad\glad.h>

namespace Saba {

	OpenGLShader::OpenGLShader(const char* vertexSrc, const char* fragmentSrc)
	{
		Init(vertexSrc, fragmentSrc);
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformFloat1(const char* name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}
	void OpenGLShader::SetUniformFloat1v(const char* name, float* value, int count)
	{
		glUniform1fv(GetUniformLocation(name), count, value);
	}
	void OpenGLShader::SetUniformFloat2(const char* name, const glm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}
	void OpenGLShader::SetUniformFloat3(const char* name, const glm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}
	void OpenGLShader::SetUniformFloat4(const char* name, const glm::vec4& value)
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformInt1(const char* name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}
	void OpenGLShader::SetUniformInt1v(const char* name, int* value, int count)
	{
		glUniform1iv(GetUniformLocation(name), count, value);
	}
	void OpenGLShader::SetUniformInt2(const char* name, const glm::ivec2& value)
	{
		glUniform2i(GetUniformLocation(name), value.x, value.y);
	}
	void OpenGLShader::SetUniformInt3(const char* name, const glm::ivec3& value)
	{
		glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
	}
	void OpenGLShader::SetUniformInt4(const char* name, const glm::ivec4& value)
	{
		glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformMat3(const char* name, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::SetUniformMat4(const char* name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::Init(const char* vertexSrc, const char* fragmentSrc)
	{
		uint vShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vShader, 1, &vertexSrc, nullptr);
		glCompileShader(vShader);

		int result;
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int lenght;
			glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &lenght);
			char* message = (char*)alloca(lenght);
			glGetShaderInfoLog(vShader, lenght, &lenght, message);
			glDeleteShader(vShader);
			SB_CORE_FATAL("Failed to compile vertex shader");
			SB_CORE_ASSERT(false, "{0}", message);
		}

		uint fShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fShader, 1, &fragmentSrc, nullptr);
		glCompileShader(fShader);

		glGetShaderiv(fShader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int lenght;
			glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &lenght);
			char* message = (char*)alloca(lenght);
			glGetShaderInfoLog(fShader, lenght, &lenght, message);
			glDeleteShader(vShader);
			glDeleteShader(fShader);
			SB_CORE_FATAL("Failed to compile fragment shader");
			SB_CORE_ASSERT(false, "{0}", message);
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vShader);
		glAttachShader(m_ID, fShader);
		glLinkProgram(m_ID);

		glDetachShader(m_ID, vShader);
		glDetachShader(m_ID, fShader);
		glDeleteShader(vShader);
		glDeleteShader(fShader);

		glGetProgramiv(m_ID, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int lenght;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &lenght);
			char* message = (char*)alloca(lenght);
			glGetProgramInfoLog(m_ID, lenght, &lenght, message);
			glDeleteProgram(m_ID);
			SB_CORE_FATAL("Failed to link shader");
			SB_CORE_ASSERT(false, "{0}", message);
		}
	}
	int OpenGLShader::GetUniformLocation(const char* name)
	{
		if (m_UniformLocations.find(name) != m_UniformLocations.end())
			return m_UniformLocations[name];
		
		int location = glGetUniformLocation(m_ID, name);
		if (location == -1) SB_CORE_WARN(std::string("Uniform (") + name + ") does not exist!");
		m_UniformLocations[name] = location;
		return location;
	}
}
