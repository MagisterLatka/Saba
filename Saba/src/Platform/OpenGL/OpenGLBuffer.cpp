#include "pch.h"
#include "OpenGLBuffer.h"
#include "OpenGLError.h"

namespace Saba {

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* data, uint32_t size, BufferUsage usage)
	{
		GLCall(glCreateBuffers(1, &m_ID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage == Static ? GL_STATIC_DRAW : usage == Dynamic ? GL_DYNAMIC_DRAW : usage == Stream ? GL_STREAM_DRAW : 0));
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void OpenGLVertexBuffer::SetData(void* data, uint32_t size, uint32_t offset)
	{
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}



	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* data, uint32_t count, BufferUsage usage)
		: m_Count(count)
	{
		GLCall(glCreateBuffers(1, &m_ID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, usage == Static ? GL_STATIC_DRAW : usage == Dynamic ? GL_DYNAMIC_DRAW : usage == Stream ? GL_STREAM_DRAW : 0));
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void OpenGLIndexBuffer::SetData(void* data, uint32_t size, uint32_t offset)
	{
		GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
	}



	OpenGLUniformBuffer::OpenGLUniformBuffer(void* data, uint32_t size, BufferUsage usage)
	{
		GLCall(glCreateBuffers(1, &m_ID));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
		GLCall(glBufferData(GL_UNIFORM_BUFFER, size, data, usage == Static ? GL_STATIC_DRAW : usage == Dynamic ? GL_DYNAMIC_DRAW : usage == Stream ? GL_STREAM_DRAW : 0));
	}
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLUniformBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	}
	void OpenGLUniformBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void OpenGLUniformBuffer::SetBinding(uint8_t binding)
	{
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID));
	}

	void OpenGLUniformBuffer::SetData(void* data, uint32_t size, uint32_t offset)
	{
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
	}
}