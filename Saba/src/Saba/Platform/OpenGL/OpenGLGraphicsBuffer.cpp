#include <pch.h>
#include "OpenGLGraphicsBuffers.h"

#include "Saba/Renderer/Renderer.h"

#include <glad/glad.h>

namespace Saba {

static GLenum GetUsage(BufferUsage usage)
{
    switch (usage)
    {
        case BufferUsage::Default:      return GL_STATIC_DRAW;
        case BufferUsage::Dynamic:      return GL_DYNAMIC_DRAW;
        case BufferUsage::Immutable:    return GL_STATIC_DRAW;
        default: break;
    }
    SB_CORE_THROW_INFO("Invalid buffer usage");
    return 0;
}

OpenGLVertexBuffer::OpenGLVertexBuffer(BufferLayout layout, const void* data, uint32_t size, BufferUsage usage)
    : m_Layout(std::move(layout)), m_Size(size), m_Usage(usage)
{
    if (data != nullptr)
        m_Data = Buffer::Copy(data, size);
    Create();
}
OpenGLVertexBuffer::OpenGLVertexBuffer(BufferLayout layout, const Buffer& buffer, BufferUsage usage)
    : m_Layout(std::move(layout)), m_Data(buffer), m_Size(buffer.Size), m_Usage(usage)
{
    Create();
}
OpenGLVertexBuffer::OpenGLVertexBuffer(BufferLayout layout, Buffer&& buffer, BufferUsage usage)
    : m_Layout(std::move(layout)), m_Data(std::move(buffer)), m_Size(buffer.Size), m_Usage(usage)
{
    Create();
}
OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &m_ID);
}
void OpenGLVertexBuffer::Create() {
    Ref<OpenGLVertexBuffer> instance = this;
    Renderer::Submit([instance]() mutable {
        glCreateBuffers(1, &instance->m_ID);
        if (instance->m_Usage == BufferUsage::Immutable)
            glNamedBufferStorage(instance->m_ID, instance->m_Size, instance->m_Data.Data, 0);
        else
            glNamedBufferData(instance->m_ID, instance->m_Size, instance->m_Data.Data, GetUsage(instance->m_Usage));
    });
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
{
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    SB_CORE_ASSERT(size + offset <= m_Size, "Vertex buffer overflow");
    m_Data = Buffer::Copy(data, size);
    Update(offset);
}
void OpenGLVertexBuffer::SetData(const Buffer& buffer, uint32_t offset)
{
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    SB_CORE_ASSERT(buffer.Size + offset <= m_Size, "Vertex buffer overflow");
    m_Data = buffer;
    Update(offset);
}
void OpenGLVertexBuffer::SetData(Buffer&& buffer, uint32_t offset)
{
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    SB_CORE_ASSERT(buffer.Size + offset <= m_Size, "Vertex buffer overflow");
    m_Data = std::move(buffer);
    Update(offset);
}
void OpenGLVertexBuffer::Update(uint32_t offset)
{
    Ref<OpenGLVertexBuffer> instance = this;
    Renderer::Submit([instance, offset]() {
        if (instance->m_Usage == BufferUsage::Dynamic) {
            void* data = glMapNamedBufferRange(instance->m_ID, offset, instance->m_Data.Size, GL_MAP_WRITE_BIT);
            memcpy(data, instance->m_Data.Data, instance->m_Data.Size);
            glUnmapNamedBuffer(instance->m_ID);
        }
        else {
            glNamedBufferSubData(instance->m_ID, offset, instance->m_Data.Size, instance->m_Data.Data);
        }
    });
}
void OpenGLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, uint32_t size, BufferUsage usage)
    : m_Size(size), m_Usage(usage)
{
    if (data != nullptr)
        m_Data = Buffer::Copy(data, size);
    Create();
}
OpenGLIndexBuffer::OpenGLIndexBuffer(const Buffer& buffer, BufferUsage usage)
    : m_Data(buffer), m_Size(buffer.Size), m_Usage(usage)
{
    Create();
}
OpenGLIndexBuffer::OpenGLIndexBuffer(Buffer&& buffer, BufferUsage usage)
    : m_Data(std::move(buffer)), m_Size(buffer.Size), m_Usage(usage)
{
    Create();
}
OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &m_ID);
}
void OpenGLIndexBuffer::Create() {
    Ref<OpenGLIndexBuffer> instance = this;
    Renderer::Submit([instance]() mutable {
        glCreateBuffers(1, &instance->m_ID);
        if (instance->m_Usage == BufferUsage::Immutable)
            glNamedBufferStorage(instance->m_ID, instance->m_Size, instance->m_Data.Data, 0);
        else
            glNamedBufferData(instance->m_ID, instance->m_Size, instance->m_Data.Data, GetUsage(instance->m_Usage));
    });
}

void OpenGLIndexBuffer::SetData(const uint32_t* data, uint32_t size, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    SB_CORE_ASSERT(size + offset <= m_Size, "Index buffer overflow");
    m_Data = Buffer::Copy(data, size);
    Update(offset);
}
void OpenGLIndexBuffer::SetData(const Buffer& buffer, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    SB_CORE_ASSERT(buffer.Size + offset <= m_Size, "Index buffer overflow");
    m_Data = buffer;
    Update(offset);
}
void OpenGLIndexBuffer::SetData(Buffer&& buffer, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    SB_CORE_ASSERT(buffer.Size + offset <= m_Size, "Index buffer overflow");
    m_Data = std::move(buffer);
    Update(offset);
}
void OpenGLIndexBuffer::Update(uint32_t offset) {
    Ref<OpenGLIndexBuffer> instance = this;
    Renderer::Submit([instance, offset]() {
        if (instance->m_Usage == BufferUsage::Dynamic) {
            void* data = glMapNamedBufferRange(instance->m_ID, offset, instance->m_Data.Size, GL_MAP_WRITE_BIT);
            memcpy(data, instance->m_Data.Data, instance->m_Data.Size);
            glUnmapNamedBuffer(instance->m_ID);
        }
        else {
            glNamedBufferSubData(instance->m_ID, offset, instance->m_Data.Size, instance->m_Data.Data);
        }
    });
}
void OpenGLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


OpenGLConstantBuffer::OpenGLConstantBuffer([[maybe_unused]] BufferShaderBinding binding, const void* data, uint32_t size)
    : m_Size(size)
{
    if (data != nullptr)
        m_Data = Buffer::Copy(data, size);
    Create();
}
OpenGLConstantBuffer::OpenGLConstantBuffer([[maybe_unused]] BufferShaderBinding binding, const Buffer& buffer)
    : m_Data(buffer), m_Size(buffer.Size)
{
    Create();
}
OpenGLConstantBuffer::OpenGLConstantBuffer([[maybe_unused]] BufferShaderBinding binding, Buffer&& buffer)
    : m_Data(std::move(buffer)), m_Size(buffer.Size)
{
    Create();
}
OpenGLConstantBuffer::OpenGLConstantBuffer([[maybe_unused]] BufferShaderBinding binding, const UniformBufferBase& buffer)
    : m_Size(buffer.GetBufferSize())
{
    m_Data = Buffer::Copy(buffer.GetBuffer(), buffer.GetBufferSize());
    Create();
}
OpenGLConstantBuffer::~OpenGLConstantBuffer() {
    glDeleteBuffers(1, &m_ID);
}
void OpenGLConstantBuffer::Create() {
    Ref<OpenGLConstantBuffer> instance = this;
    Renderer::Submit([instance]() mutable {
        glCreateBuffers(1, &instance->m_ID);
        glNamedBufferStorage(instance->m_ID, instance->m_Size, instance->m_Data.Data, GL_DYNAMIC_STORAGE_BIT);
    });
}

void OpenGLConstantBuffer::SetData(const void* data, uint32_t size) {
    SB_CORE_ASSERT(size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = Buffer::Copy(data, size);
    Update();
}
void OpenGLConstantBuffer::SetData(const Buffer& buffer) {
    SB_CORE_ASSERT(buffer.Size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = buffer;
    Update();
}
void OpenGLConstantBuffer::SetData(Buffer&& buffer) {
    SB_CORE_ASSERT(buffer.Size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = std::move(buffer);
    Update();
}
void OpenGLConstantBuffer::SetData(const UniformBufferBase& buffer) {
    SB_CORE_ASSERT(buffer.GetBufferSize() == m_Size, "Only updating whole constant buffer is possible");
    m_Data = Buffer::Copy(buffer.GetBuffer(), buffer.GetBufferSize());
    Update();
}
void OpenGLConstantBuffer::Update() {
    Ref<OpenGLConstantBuffer> instance = this;
    Renderer::Submit([instance]() {
        glNamedBufferSubData(instance->m_ID, 0, instance->m_Size, instance->m_Data.Data);
    });
}

void OpenGLConstantBuffer::Bind(uint32_t slot) const {
    Ref<const OpenGLConstantBuffer> instance = this;
    Renderer::Submit([instance, slot]() {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, instance->m_ID);
    });
}

}
