#include "pch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Saba {

	static void OpenGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		const char* Source;

		switch (source)
		{
			case GL_DEBUG_SOURCE_API:
				Source = "API";
				break;

			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				Source = "WINDOW SYSTEM";
				break;

			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				Source = "SHADER COMPILER";
				break;

			case GL_DEBUG_SOURCE_THIRD_PARTY:
				Source = "THIRD PARTY";
				break;

			case GL_DEBUG_SOURCE_APPLICATION:
				Source = "APPLICATION";
				break;

			case GL_DEBUG_SOURCE_OTHER:
			default:
				Source = "UNKNOWN";
				break;
		}

		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:			SB_CORE_FATAL("({0}): {1}", Source, message);		break;
			case GL_DEBUG_SEVERITY_MEDIUM:			SB_CORE_ERROR("({0}): {1}", Source, message);		break;
			case GL_DEBUG_SEVERITY_LOW:				SB_CORE_WARN("({0}): {1}", Source, message);		break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:	SB_CORE_TRACE("({0}): {1}", Source, message);		break;
			default:								SB_CORE_ASSERT(false, "Unknown severity level!");	break;
		}		
		SB_BREAK();
	}

	void OpenGLRendererAPI::Init()
	{
#ifdef SB_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::EnableDepthTest(bool enable)
	{
		glDepthMask(enable ? GL_TRUE : GL_FALSE);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indicesCount)
	{
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, indicesCount ? indicesCount : vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
