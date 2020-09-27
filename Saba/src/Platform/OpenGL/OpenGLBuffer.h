#pragma once

#include "Saba/Renderer/Buffer.h"

namespace Saba {

	class OpenGLVertexBuffer : public VertexBuffer
	{
		friend class OpenGLVertexArray;
	public:
		OpenGLVertexBuffer(void* data, uint32_t size, BufferUsage usage);
		virtual ~OpenGLVertexBuffer();

		virtual void SetData(void* data, uint32_t size, uint32_t offset) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override;
	private:
		void Bind() const;
	private:
		uint32_t m_ID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
		friend class OpenGLVertexArray;
	public:
		OpenGLIndexBuffer(uint32_t* data, uint32_t count, BufferUsage usage);
		virtual ~OpenGLIndexBuffer();

		virtual void SetData(uint32_t* data, uint32_t count, uint32_t offset) override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		void Bind() const;
	private:
		uint32_t m_ID;
		uint32_t m_Count;
	};

}
