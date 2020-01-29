#include "pch.h"
#include "Buffer.h"

#include "Platform\OpenGL\OpenGLBuffer.h"

namespace Saba {

	VertexBuffer* VertexBuffer::Create(float* data, uint size)
	{
#if defined(SB_USE_OPENGL)
		return new OpenGLVertexBuffer(data, size);
#endif
	}

	IndexBuffer* IndexBuffer::Create(uint* data, uint size)
	{
#if defined(SB_USE_OPENGL)
		return new OpenGLIndexBuffer(data, size);
#endif
	}

}
