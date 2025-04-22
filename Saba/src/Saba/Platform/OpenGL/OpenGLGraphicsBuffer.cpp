#include <pch.h>
#include "OpenGLGraphicsBuffers.h"

#include "Saba/Renderer/Renderer.h"

#include <glad/glad.h>

namespace Saba {

OpenGLGraphicsBuffer::OpenGLGraphicsBuffer(const void* data, uint32_t size, GraphicsBufferUsage usage)
    : m_Size(size), m_Usage(usage)
{
    if (data != nullptr)
        m_Data = Buffer::Copy(data, size);
    Create();
}
OpenGLGraphicsBuffer::OpenGLGraphicsBuffer(const Buffer& buffer, GraphicsBufferUsage usage)
    : m_Data(buffer), m_Size(buffer.Size), m_Usage(usage)
{
    Create();
}
OpenGLGraphicsBuffer::OpenGLGraphicsBuffer(Buffer&& buffer, GraphicsBufferUsage usage)
    : m_Data(std::move(buffer)), m_Size(m_Data.Size), m_Usage(usage)
{
    Create();
}
OpenGLGraphicsBuffer::~OpenGLGraphicsBuffer() {
    glDeleteBuffers(1, &m_ID);
}

void OpenGLGraphicsBuffer::Create() {
    SB_CORE_ASSERT(m_Usage > GraphicsBufferUsage::None && m_Usage <= GraphicsBufferUsage::Last,
        "Invalid graphics buffer usage: {0}", static_cast<uint32_t>(m_Usage));

    Ref<OpenGLGraphicsBuffer> instance = this;
    Renderer::Submit([instance]() mutable {
        glCreateBuffers(1, &instance->m_ID);
        uint32_t flags = 0u;
        if (instance->m_Usage == GraphicsBufferUsage::Default) {
            flags |= GL_DYNAMIC_STORAGE_BIT;
        }
        else if (instance->m_Usage == GraphicsBufferUsage::Dynamic) {
            flags |= GL_MAP_WRITE_BIT;
        }
        else if (instance->m_Usage == GraphicsBufferUsage::Staging) {
            flags |= GL_MAP_WRITE_BIT | GL_CLIENT_STORAGE_BIT;
        }
        glNamedBufferStorage(instance->m_ID, instance->m_Size, instance->m_Data.Data, flags);
    });
}

void OpenGLGraphicsBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != GraphicsBufferUsage::Immutable, "Cannot modify immutable graphics buffer");
    SB_CORE_ASSERT(size + offset <= m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        size, offset, m_Size);
    m_Data = Buffer::Copy(data, size);
    Update(offset);
}
void OpenGLGraphicsBuffer::SetData(const Buffer& buffer, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != GraphicsBufferUsage::Immutable, "Cannot modify immutable graphics buffer");
    SB_CORE_ASSERT(m_Data.Size + offset <= m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        m_Data.Size, offset, m_Size);
    m_Data = buffer;
    Update(offset);
}
void OpenGLGraphicsBuffer::SetData(Buffer&& buffer, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != GraphicsBufferUsage::Immutable, "Cannot modify immutable graphics buffer");
    SB_CORE_ASSERT(m_Data.Size + offset <= m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        m_Data.Size, offset, m_Size);
    m_Data = std::move(buffer);
    Update(offset);
}
void OpenGLGraphicsBuffer::UploadCurrent(uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != GraphicsBufferUsage::Immutable, "Cannot modify immutable graphics buffer");
    SB_CORE_ASSERT(m_Data.Size + offset <= m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        m_Data.Size, offset, m_Size);
    Update(offset);
}
void OpenGLGraphicsBuffer::Update(uint32_t offset) {
    Ref<OpenGLGraphicsBuffer> instance = this;
    Renderer::Submit([instance, offset]() {
        if (instance->m_Usage == GraphicsBufferUsage::Dynamic || instance->m_Usage == GraphicsBufferUsage::Staging) {
            uint32_t flags = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
            void* data = glMapNamedBufferRange(instance->m_ID, offset, instance->m_Data.Size, flags);
            memcpy(data, instance->m_Data.Data, instance->m_Data.Size);
            glUnmapNamedBuffer(instance->m_ID);
        }
        else {
            glNamedBufferSubData(instance->m_ID, offset, instance->m_Data.Size, instance->m_Data.Data);
        }
    });
}

void OpenGLGraphicsBuffer::TransferToBuffer(Ref<GraphicsBuffer> other, uint32_t dataSize, uint32_t offset, uint32_t otherOffset) {
    SB_CORE_ASSERT(other.CanConvert<OpenGLGraphicsBuffer>(), "Using TransferToBuffer function is only available between same RendererAPIs");
    auto second = other.As<OpenGLGraphicsBuffer>();
    SB_CORE_ASSERT(offset + dataSize <= m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        dataSize, offset, m_Size);
    SB_CORE_ASSERT(otherOffset + dataSize <= second->m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
            dataSize, otherOffset, second->m_Size);

    Ref<OpenGLGraphicsBuffer> instance = this;
    Renderer::Submit([instance, second, dataSize, offset, otherOffset]() {
        glCopyNamedBufferSubData(instance->m_ID, second->m_ID, offset, otherOffset, dataSize);
    });
}



void OpenGLConstantBuffer::SetData(const void* data, uint32_t size, [[maybe_unused]] uint32_t offset) {
    SB_CORE_ASSERT(size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = Buffer::Copy(data, size);
    Update(0u);
}
void OpenGLConstantBuffer::SetData(const Buffer& buffer, [[maybe_unused]] uint32_t offset) {
    SB_CORE_ASSERT(buffer.Size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = buffer;
    Update(0u);
}
void OpenGLConstantBuffer::SetData(Buffer&& buffer, [[maybe_unused]] uint32_t offset) {
    SB_CORE_ASSERT(buffer.Size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = std::move(buffer);
    Update(0u);
}
void OpenGLConstantBuffer::SetData(const UniformBufferBase& buffer) {
    SB_CORE_ASSERT(buffer.GetBufferSize() == m_Size, "Only updating whole constant buffer is possible");
    m_Data = Buffer::Copy(buffer.GetBuffer(), buffer.GetBufferSize());
    Update(0u);
}

void OpenGLConstantBuffer::Bind(uint32_t slot) const {
    Ref<const OpenGLConstantBuffer> instance = this;
    Renderer::Submit([instance, slot]() {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, instance->m_ID);
    });
}

} //namespace Saba
