#include "pch.h"
#include "Shader.h"

#include "Platform\OpenGL\OpenGLShader.h"

namespace Saba {

	Shader* Shader::Create(const char* vertexSrc, const char* fragmentSrc)
	{
#if defined(SB_USE_OPENGL)
		return new OpenGLShader(vertexSrc, fragmentSrc);
#endif
	}

}
