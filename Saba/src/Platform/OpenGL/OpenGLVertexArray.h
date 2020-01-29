#pragma once

#include "Saba\Renderer\VertexArray.h"

namespace Saba {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
		virtual void SetVertexBuffer(const std::shared_ptr<IndexBuffer>& buffer) override;

		inline virtual const std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() const override { return m_VertexBuffers; }
		inline virtual const std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		uint m_ID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		uint m_Index = 0;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}
