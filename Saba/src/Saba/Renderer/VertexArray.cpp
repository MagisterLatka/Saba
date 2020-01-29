#include "pch.h"
#include "VertexArray.h"

#include "Platform\OpenGL\OpenGLVertexArray.h"

namespace Saba {

	VertexArray* VertexArray::Create()
	{
#if defined(SB_USE_OPENGL)
		return new OpenGLVertexArray;
#endif
	}

}
