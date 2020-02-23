#include "pch.h"
#include "OpenGLBuffer.h"

#include <glad\glad.h>

namespace Saba {

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* data, uint32_t size, BufferUsage usage)
	{
		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage == Static ? GL_STATIC_DRAW : usage == Dynamic ? GL_DYNAMIC_DRAW : usage == Stream ? GL_STREAM_DRAW : 0);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void* OpenGLVertexBuffer::Map() const
	{
		return glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	}
	void OpenGLVertexBuffer::Unmap() const
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void OpenGLVertexBuffer::SetSubdata(uint32_t offset, uint32_t size, void* data)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}



	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* data, uint32_t count, BufferUsage usage)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, usage == Static ? GL_STATIC_DRAW : usage == Dynamic ? GL_DYNAMIC_DRAW : usage == Stream ? GL_STREAM_DRAW : 0);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void* OpenGLIndexBuffer::Map() const
	{
		return glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	}
	void OpenGLIndexBuffer::Unmap() const
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void OpenGLIndexBuffer::SetSubdata(uint32_t offset, uint32_t size, void* data)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

}