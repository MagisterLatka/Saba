#include "pch.h"
#include "OpenGLVertexArray.h"

#include <glad\glad.h>

namespace Saba {

	static uint ShaderDataTypeToOpenGLBaseType(Saba::ShaderDataType type)
	{
		switch (type)
		{
			case Saba::ShaderDataType::Float:    return GL_FLOAT;
			case Saba::ShaderDataType::Float2:   return GL_FLOAT;
			case Saba::ShaderDataType::Float3:   return GL_FLOAT;
			case Saba::ShaderDataType::Float4:   return GL_FLOAT;
			case Saba::ShaderDataType::Mat3:     return GL_FLOAT;
			case Saba::ShaderDataType::Mat4:     return GL_FLOAT;
			case Saba::ShaderDataType::Int:      return GL_INT;
			case Saba::ShaderDataType::Int2:     return GL_INT;
			case Saba::ShaderDataType::Int3:     return GL_INT;
			case Saba::ShaderDataType::Int4:     return GL_INT;
			case Saba::ShaderDataType::Bool:     return GL_BOOL;
		}
			
		SB_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_ID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer)
	{
		glBindVertexArray(m_ID);
		buffer->Bind();

		for (const auto& element : buffer->GetLayout())
		{
			glEnableVertexAttribArray(m_Index);
			glVertexAttribPointer(m_Index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized, buffer->GetLayout().GetStride(), (const void*)element.Offset);
			m_Index++;
		}

		m_VertexBuffers.push_back(buffer);
	}
	void OpenGLVertexArray::SetVertexBuffer(const std::shared_ptr<IndexBuffer>& buffer)
	{
		glBindVertexArray(m_ID);
		buffer->Bind();

		m_IndexBuffer = buffer;
	}

}