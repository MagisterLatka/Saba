#pragma once

#include "Saba\Renderer\Buffer.h"

#include <glad\glad.h>

namespace Saba {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* data, uint32_t size, BufferUsage usage);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(void* data, uint32_t size, uint32_t offset) override;

		inline virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override;
	private:
		uint32_t m_ID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* data, uint32_t count, BufferUsage usage);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(void* data, uint32_t size, uint32_t offset) override;

		inline uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_ID;
		uint32_t m_Count;
	};

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(void* data, uint32_t size, BufferUsage usage);
		virtual ~OpenGLUniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetBinding(uint8_t binding) override;

		virtual void SetData(void* data, uint32_t size, uint32_t offset) override;
	private:
		uint32_t m_ID;
	};
}
