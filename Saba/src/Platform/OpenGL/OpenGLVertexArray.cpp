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

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
	{
		glBindVertexArray(m_ID);
		buffer->Bind();

		for (const auto& element : buffer->GetLayout())
		{
			if (element.Type == ShaderDataType::Mat3)
			{
				for (int i = 0; i < 3; i++)
				{
					glEnableVertexAttribArray(m_Index);
					glVertexAttribPointer(m_Index, 3, GL_FLOAT, element.Normalized, buffer->GetLayout().GetStride(), (const void*)element.Offset);
					m_Index++;
				}
			}
			else if (element.Type == ShaderDataType::Mat4)
			{
				for (int i = 0; i < 4; i++)
				{
					glEnableVertexAttribArray(m_Index);
					glVertexAttribPointer(m_Index, 4, GL_FLOAT, element.Normalized, buffer->GetLayout().GetStride(), (const void*)element.Offset);
					m_Index++;
				}
			}
			else
			{
				glEnableVertexAttribArray(m_Index);
				glVertexAttribPointer(m_Index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized, buffer->GetLayout().GetStride(), (const void*)element.Offset);
				m_Index++;
			}
		}

		m_VertexBuffers.emplace_back(buffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
	{
		glBindVertexArray(m_ID);
		buffer->Bind();

		m_IndexBuffer = buffer;
	}

}
