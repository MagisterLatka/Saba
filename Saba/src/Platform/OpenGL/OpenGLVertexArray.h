#pragma once

#include "Saba/Renderer/VertexArray.h"

namespace Saba {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual Ref<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		uint32_t m_ID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		uint32_t m_Index = 0;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}
