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

        if (instance->m_IndexBuffer)
            glVertexArrayElementBuffer(instance->m_ID, instance->m_IndexBuffer->m_ID);

        uint32_t index = 0u;
        for (const auto& [id, buffer] : instance->m_VertexBuffers | std::views::enumerate) {
            glVertexArrayVertexBuffer(instance->m_ID, id, buffer->m_ID, 0,
                static_cast<int>(buffer->GetLayout().GetStride()));

            SB_CORE_ASSERT(!buffer->GetLayout().GetElements().empty(),
                "Vertex buffer's layout must have at least one element");
            uint32_t instanceStepRate = buffer->GetLayout().GetElements().at(0).InstanceStepRate;
            glVertexArrayBindingDivisor(instance->m_ID, id, instanceStepRate);

            for (const auto& element : buffer->GetLayout()) {
                SB_CORE_ASSERT(instanceStepRate == element.InstanceStepRate,
                    "Vertex buffer's layout must have consistent instance step rate");
                switch (element.Type) {
                    case BufferLayoutElementDataType::Mat3:
                        for (int i = 0; i < 3; ++i) {
                            glEnableVertexArrayAttrib(instance->m_ID, index);
                            glVertexArrayAttribFormat(instance->m_ID, index, 3, GL_FLOAT, GL_FALSE,
                                element.Offset + sizeof(float) * 3 * static_cast<uint64_t>(i));
                            glVertexArrayAttribBinding(instance->m_ID, index, id);
                            ++index;
                        }
                        break;
                    case BufferLayoutElementDataType::Mat4:
                        for (int i = 0; i < 4; ++i) {
                            glEnableVertexArrayAttrib(instance->m_ID, index);
                            glVertexArrayAttribFormat(instance->m_ID, index, 4, GL_FLOAT, GL_FALSE,
                                element.Offset + sizeof(float) * 4 * static_cast<uint64_t>(i));
                            glVertexArrayAttribBinding(instance->m_ID, index, id);
                            ++index;
                        }
                        break;
                    case BufferLayoutElementDataType::Float:
                    case BufferLayoutElementDataType::Float2:
                    case BufferLayoutElementDataType::Float3:
                    case BufferLayoutElementDataType::Float4:
                        glEnableVertexArrayAttrib(instance->m_ID, index);
                        glVertexArrayAttribFormat(instance->m_ID, index, static_cast<int>(element.GetComponentCount()), GL_FLOAT,
                            element.Normalized, static_cast<uint64_t>(element.Offset));
                        glVertexArrayAttribBinding(instance->m_ID, index, id);
                        ++index;
                        break;
                    case BufferLayoutElementDataType::Int:
                    case BufferLayoutElementDataType::Int2:
                    case BufferLayoutElementDataType::Int3:
                    case BufferLayoutElementDataType::Int4:
                        glEnableVertexArrayAttrib(instance->m_ID, index);
                        glVertexArrayAttribIFormat(instance->m_ID, index, static_cast<int>(element.GetComponentCount()), GL_INT,
                            static_cast<uint64_t>(element.Offset));
                        glVertexArrayAttribBinding(instance->m_ID, index, id);
                        ++index;
                        break;
                    case BufferLayoutElementDataType::UInt:
                    case BufferLayoutElementDataType::UInt2:
                    case BufferLayoutElementDataType::UInt3:
                    case BufferLayoutElementDataType::UInt4:
                        glEnableVertexArrayAttrib(instance->m_ID, index);
                        glVertexArrayAttribIFormat(instance->m_ID, index, static_cast<int>(element.GetComponentCount()), GL_UNSIGNED_INT,
                            static_cast<uint64_t>(element.Offset));
                        glVertexArrayAttribBinding(instance->m_ID, index, id);
                        ++index;
                        break;
                        break;
                    default: break;
                }
            }
        }
    });
}

void OpenGLInputLayout::Bind() const noexcept {
    Ref<const OpenGLInputLayout> instance = this;
    Renderer::Submit([instance]() {
        glBindVertexArray(instance->m_ID);
    });
}

} //namespace Saba
