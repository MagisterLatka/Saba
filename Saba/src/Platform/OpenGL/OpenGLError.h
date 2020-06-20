#pragma once

void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);

#define GLASSERT(x) if (!x) __debugbreak();
#if defined(DEBUG)
#	define GLCall(x) GLClearError();\
			x;\
			GLASSERT(GLCheckError(#x, __FILE__, __LINE__));
#else
#	define GLCall(x) x;
#endif
