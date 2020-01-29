#pragma once

#include "Saba\Renderer\Buffer.h"

namespace Saba {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* data, uint size);
		virtual ~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;
	private:
		uint m_ID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint* data, uint count);
		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		inline uint GetCount() const override { return m_Count; }
	private:
		uint m_ID;
		uint m_Count;
	};

}
