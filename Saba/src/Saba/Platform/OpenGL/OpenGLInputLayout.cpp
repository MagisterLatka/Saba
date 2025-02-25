#include <pch.h>
#include "OpenGLInputLayout.h"

#include "Saba/Renderer/Renderer.h"

#include <glad/glad.h>

namespace Saba {

OpenGLInputLayout::OpenGLInputLayout(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers,
    [[maybe_unused]] const Ref<Shader>& shader, const Ref<IndexBuffer>& indexBuffer)
    : m_IndexBuffer(indexBuffer.As<OpenGLIndexBuffer>())
{
    SB_CORE_ASSERT(vertexBuffers.size() > 0, "Input layout requires at least one vertex buffer");
    for (const auto& buffer : vertexBuffers)
        m_VertexBuffers.push_back(buffer.As<OpenGLVertexBuffer>());
    Create();
}
OpenGLInputLayout::~OpenGLInputLayout() {
    glDeleteVertexArrays(1, &m_ID);
}
void OpenGLInputLayout::Create() {
    Ref<OpenGLInputLayout> instance = this;
    Renderer::Submit([instance]() mutable {
        glCreateVertexArrays(1, &instance->m_ID);
        glBindVertexArray(instance->m_ID);
        for (const auto& buffer : instance->m_VertexBuffers)
            buffer->Bind();
        if (instance->m_IndexBuffer)
            instance->m_IndexBuffer->Bind();

        uint32_t index = 0u;
        for (const auto& buffer : instance->m_VertexBuffers) {
            for (const auto& element : buffer->GetLayout()) {
                switch (element.Type) {
                    case BufferLayoutElementDataType::Mat3:
                        for (int i = 0; i < 3; ++i) {
                            glEnableVertexAttribArray(index);
                            glVertexAttribPointer(index++, 3, GL_FLOAT, element.Normalized, static_cast<int>(buffer->GetLayout().GetStride()),
                                (const void*)(static_cast<uint64_t>(element.Offset + sizeof(float) * 3 * static_cast<uint64_t>(i))));
                        }
                        break;
                    case BufferLayoutElementDataType::Mat4:
                        for (int i = 0; i < 4; ++i) {
                            glEnableVertexAttribArray(index);
                            glVertexAttribPointer(index++, 4, GL_FLOAT, element.Normalized, static_cast<int>(buffer->GetLayout().GetStride()),
                                (const void*)(static_cast<uint64_t>(element.Offset + sizeof(float) * 4 * static_cast<uint64_t>(i))));
                        }
                        break;
                    case BufferLayoutElementDataType::Float:
                    case BufferLayoutElementDataType::Float2:
                    case BufferLayoutElementDataType::Float3:
                    case BufferLayoutElementDataType::Float4:
                        glEnableVertexAttribArray(index);
                        glVertexAttribPointer(index++, static_cast<int>(element.GetComponentCount()), GL_FLOAT, element.Normalized,
                            static_cast<int>(buffer->GetLayout().GetStride()), (const void*)(static_cast<uint64_t>(element.Offset)));
                        break;
                    case BufferLayoutElementDataType::Int:
                    case BufferLayoutElementDataType::Int2:
                    case BufferLayoutElementDataType::Int3:
                    case BufferLayoutElementDataType::Int4:
                        glEnableVertexAttribArray(index);
                        glVertexAttribIPointer(index++, static_cast<int>(element.GetComponentCount()), GL_INT,
                            static_cast<int>(buffer->GetLayout().GetStride()), (const void*)(static_cast<uint64_t>(element.Offset)));
                        break;
                    case BufferLayoutElementDataType::UInt:
                    case BufferLayoutElementDataType::UInt2:
                    case BufferLayoutElementDataType::UInt3:
                    case BufferLayoutElementDataType::UInt4:
                        glEnableVertexAttribArray(index);
                        glVertexAttribIPointer(index++, static_cast<int>(element.GetComponentCount()), GL_UNSIGNED_INT,
                            static_cast<int>(buffer->GetLayout().GetStride()), (const void*)(static_cast<uint64_t>(element.Offset)));
                        break;
                }
            }
        }
    });
}

void OpenGLInputLayout::Bind() const noexcept {
    Ref<const OpenGLInputLayout> instance = this;
    Renderer::Submit([instance]() {
        glBindVertexArray(instance->m_ID);
        for (const auto& buffer : instance->m_VertexBuffers)
            buffer->Bind();
        if (instance->m_IndexBuffer)
            instance->m_IndexBuffer->Bind();
    });
}

}
