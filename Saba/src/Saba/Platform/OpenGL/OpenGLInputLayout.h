#pragma once

#include "Saba/Renderer/InputLayout.h"
#include "Saba/Platform/OpenGL/OpenGLGraphicsBuffers.h"

namespace Saba {

class OpenGLInputLayout : public InputLayout {
public:
    OpenGLInputLayout(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers,
        const Ref<Shader>& shader, const Ref<IndexBuffer>& indexBuffer);
    ~OpenGLInputLayout();

    Ref<VertexBuffer> GetVertexBuffer(uint32_t index) override { return m_VertexBuffers[index].As<VertexBuffer>(); }
    Ref<IndexBuffer> GetIndexBuffer() noexcept override { return m_IndexBuffer.As<IndexBuffer>(); }

    void Bind() const noexcept override;
private:
    void Create();
private:
    std::vector<Ref<OpenGLVertexBuffer>> m_VertexBuffers;
    Ref<OpenGLIndexBuffer> m_IndexBuffer;
    uint32_t m_ID;
};

} //namespace Saba
