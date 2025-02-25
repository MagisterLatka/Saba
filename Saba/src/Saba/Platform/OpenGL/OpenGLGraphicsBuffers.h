#pragma once

#include "Saba/Renderer/GraphicsBuffers.h"

namespace Saba {

class OpenGLVertexBuffer : public VertexBuffer {
    friend class OpenGLInputLayout;
public:
    SB_CORE OpenGLVertexBuffer(BufferLayout layout, void* data, uint32_t size, BufferUsage usage);
    SB_CORE OpenGLVertexBuffer(BufferLayout layout, const Buffer& buffer, BufferUsage usage);
    SB_CORE OpenGLVertexBuffer(BufferLayout layout, Buffer&& buffer, BufferUsage usage);
    SB_CORE ~OpenGLVertexBuffer();

    SB_CORE void SetData(void* data, uint32_t size, uint32_t offset = 0u) override;
    SB_CORE void SetData(const Buffer& buffer, uint32_t offset = 0u) override;
    SB_CORE void SetData(Buffer&& buffer, uint32_t offset = 0u) override;

    SB_CORE const BufferLayout& GetLayout() const noexcept override { return m_Layout; }
    SB_CORE uint32_t GetSize() const noexcept override { return m_Size; }
private:
    SB_CORE void Create();
    SB_CORE void Update(uint32_t offset);
    SB_CORE void Bind() const;
private:
    BufferLayout m_Layout;
    Buffer m_Data;
    uint32_t m_ID = 0u, m_Size = 0u;
    BufferUsage m_Usage;
};

class OpenGLIndexBuffer : public IndexBuffer {
    friend class OpenGLInputLayout;
public:
    SB_CORE OpenGLIndexBuffer(uint32_t* data, uint32_t size, BufferUsage usage);
    SB_CORE OpenGLIndexBuffer(const Buffer& buffer, BufferUsage usage);
    SB_CORE OpenGLIndexBuffer(Buffer&& buffer, BufferUsage usage);
    SB_CORE ~OpenGLIndexBuffer();

    SB_CORE void SetData(uint32_t* data, uint32_t size, uint32_t offset = 0u) override;
    SB_CORE void SetData(const Buffer& buffer, uint32_t offset = 0u) override;
    SB_CORE void SetData(Buffer&& buffer, uint32_t offset = 0u) override;

    SB_CORE uint32_t GetSize() const noexcept override { return m_Size; }
    SB_CORE uint32_t GetIndicesCount() const noexcept override { return m_Size / sizeof(uint32_t); }
private:
    SB_CORE void Create();
    SB_CORE void Update(uint32_t offset);
    SB_CORE void Bind() const;
private:
    Buffer m_Data;
    uint32_t m_ID = 0u, m_Size = 0u;
    BufferUsage m_Usage;
};

class OpenGLConstantBuffer : public ConstantBuffer {
public:
    SB_CORE OpenGLConstantBuffer(BufferShaderBinding binding, void* data, uint32_t size);
    SB_CORE OpenGLConstantBuffer(BufferShaderBinding binding, const Buffer& buffer);
    SB_CORE OpenGLConstantBuffer(BufferShaderBinding binding, Buffer&& buffer);
    SB_CORE OpenGLConstantBuffer(BufferShaderBinding binding, const UniformBufferBase& buffer);
    SB_CORE ~OpenGLConstantBuffer();

    SB_CORE void SetData(void* data, uint32_t size) override;
    SB_CORE void SetData(const Buffer& buffer) override;
    SB_CORE void SetData(Buffer&& buffer) override;
    SB_CORE void SetData(const UniformBufferBase& buffer) override;

    SB_CORE void Bind(uint32_t slot) const override;

    SB_CORE uint32_t GetSize() const noexcept override { return m_Size; }
private:
    SB_CORE void Create();
    SB_CORE void Update();
private:
    Buffer m_Data;
    uint32_t m_ID = 0u, m_Size = 0u;
};

}
