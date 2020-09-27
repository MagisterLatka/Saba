#include "pch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Saba {

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size, BufferUsage usage)
	{
		glCreateBuffers(1, &m_ID);
		glNamedBufferData(m_ID, size, data, usage == BufferUsage::Static ? GL_STATIC_DRAW : usage == BufferUsage::Dynamic ? GL_DYNAMIC_DRAW : usage == BufferUsage::Stream ? GL_STREAM_DRAW : 0);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::SetData(void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_ID, offset, size, data);
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}



	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* data, uint32_t count, BufferUsage usage)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_ID);
		glNamedBufferData(m_ID, count * sizeof(uint32_t), data, usage == BufferUsage::Static ? GL_STATIC_DRAW : usage == BufferUsage::Dynamic ? GL_DYNAMIC_DRAW : usage == BufferUsage::Stream ? GL_STREAM_DRAW : 0);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLIndexBuffer::SetData(uint32_t* data, uint32_t count, uint32_t offset)
	{
		glNamedBufferSubData(m_ID, offset, count * sizeof(uint32_t), data);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

}