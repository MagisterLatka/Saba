#include "pch.h"
#include "OpenGLError.h"

#include <glad\glad.h>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
bool GLCheckError(const char* function, const char* file, int line)
{
	while (auto error = glGetError())
	{
		SB_CORE_ERROR(std::string("OpenGL Error [") + (error == GL_INVALID_ENUM ? "InvalidEnum" : error == GL_INVALID_VALUE ? "InvalidValue" : error == GL_INVALID_OPERATION ? "InvalidOperation"
					   : "") + "] in file " + file + " (" + std::to_string(line) + " line): " + function);
		return false;
	}

	return true;
}
