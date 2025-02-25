#pragma once

#include "Saba/Renderer/InputLayout.h"
#include "Saba/Platform/OpenGL/OpenGLGraphicsBuffers.h"

namespace Saba {

class OpenGLInputLayout : public InputLayout {
public:
    SB_CORE OpenGLInputLayout(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers,
        const Ref<Shader>& shader, const Ref<IndexBuffer>& indexBuffer);
    SB_CORE ~OpenGLInputLayout();

    SB_CORE Ref<VertexBuffer> GetVertexBuffer(uint32_t index) const override { return m_VertexBuffers[index]; }
    SB_CORE Ref<IndexBuffer> GetIndexBuffer() const noexcept override { return m_IndexBuffer; }

    SB_CORE void Bind() const noexcept override;
private:
    SB_CORE void Create();
private:
    std::vector<Ref<OpenGLVertexBuffer>> m_VertexBuffers;
    Ref<OpenGLIndexBuffer> m_IndexBuffer;
    uint32_t m_ID;
};

}
