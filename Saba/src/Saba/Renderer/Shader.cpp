#include "pch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "Platform\OpenGL\OpenGLShader.h"

namespace Saba {

	Shader* Shader::Create(const char* vertexSrc, const char* fragmentSrc)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SB_CORE_ASSERT(false, "None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		SB_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

}
