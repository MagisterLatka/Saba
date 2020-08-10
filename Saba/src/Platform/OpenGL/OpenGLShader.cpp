#include "pch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Saba {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		SB_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		Compile(PreProcess(Read(filepath)));
	}
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
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

	void OpenGLShader::SetUniformFloat1(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}
	void OpenGLShader::SetUniformFloat1v(const std::string& name, float* value, int count)
	{
		glUniform1fv(GetUniformLocation(name), count, value);
	}
	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}
	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}
	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformInt1(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}
	void OpenGLShader::SetUniformInt1v(const std::string& name, int* value, int count)
	{
		glUniform1iv(GetUniformLocation(name), count, value);
	}
	void OpenGLShader::SetUniformInt2(const std::string& name, const glm::ivec2& value)
	{
		glUniform2i(GetUniformLocation(name), value.x, value.y);
	}
	void OpenGLShader::SetUniformInt3(const std::string& name, const glm::ivec3& value)
	{
		glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
	}
	void OpenGLShader::SetUniformInt4(const std::string& name, const glm::ivec4& value)
	{
		glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	std::string OpenGLShader::Read(const std::string& filepath)
	{
		std::string result;
		std::ifstream stream(filepath, std::ios::in | std::ios::binary);
		if (stream)
		{
			stream.seekg(0, std::ios::end);
			size_t size = stream.tellg();
			if (size != -1)
			{
				result.resize(size);
				stream.seekg(0);
				stream.read(&result[0], size);
				stream.close();
			}
			else
			{
				SB_CORE_ASSERT(false, "Failed to read shader");
			}
		}
		else
		{
			SB_CORE_ASSERT(false, "Failed to open shader!");
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> sources;

		const char* token = "#type";
		size_t tokenLength = strlen(token);
		size_t pos = source.find(token, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			SB_CORE_ASSERT((eol != std::string::npos), "Syntax error in shader source");
			size_t begin = pos + tokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			SB_CORE_ASSERT((nextLinePos != std::string::npos), "Syntax error in shader source");
			pos = source.find(token, nextLinePos);

			sources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return sources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& sources)
	{
		m_ID = glCreateProgram();
		GLenum* shaders = (GLenum*)alloca(sources.size() * sizeof(GLenum));
		int index = 0;

		for (auto [type, source] : sources)
		{
			GLenum shader = glCreateShader(type);

			const GLchar* src = source.c_str();
			glShaderSource(shader, 1, &src, nullptr);
			glCompileShader(shader);

			int result;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				int lenght;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &lenght);
				char* message = (char*)malloc(lenght);
				glGetShaderInfoLog(shader, lenght, &lenght, message);
				glDeleteShader(shader);
				SB_CORE_FATAL("Failed to compile shader");
				SB_CORE_ASSERT(false, "{0}", message);
				free(message);
			}

			glAttachShader(m_ID, shader);
			glDeleteShader(shader); //Only marked as TO_DELETE, will be destroyed when detached
			shaders[index++] = shader;
		}

		glLinkProgram(m_ID);
		for (int i = 0; i < index; i++)
			glDetachShader(m_ID, shaders[i]);

		int result;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int lenght;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &lenght);
			char* message = (char*)malloc(lenght);
			glGetProgramInfoLog(m_ID, lenght, &lenght, message);
			glDeleteProgram(m_ID);
			SB_CORE_FATAL("Failed to link shader");
			SB_CORE_ASSERT(false, "{0}", message);
			free(message);
		}
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocations.find(name) != m_UniformLocations.end())
			return m_UniformLocations[name];
		
		int location = glGetUniformLocation(m_ID, name.c_str());
		if (location == -1)
		{
			SB_CORE_WARN("Uniform (" + name + ") does not exist!");
		}
		m_UniformLocations[name] = location;
		return location;
	}
}
