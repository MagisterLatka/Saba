#include "pch.h"
#include "OpenGLShader.h"

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

}