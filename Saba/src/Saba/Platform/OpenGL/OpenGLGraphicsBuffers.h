#pragma once

#include "Saba/Renderer/GraphicsBuffers.h"

namespace Saba {

class OpenGLGraphicsBuffer : virtual public GraphicsBuffer {
public:
    OpenGLGraphicsBuffer(const void* data, uint32_t size, GraphicsBufferUsage usage);
    OpenGLGraphicsBuffer(const Buffer& buffer, GraphicsBufferUsage usage);
    OpenGLGraphicsBuffer(Buffer&& buffer, GraphicsBufferUsage usage);
    virtual ~OpenGLGraphicsBuffer();

    void SetData(const void* data, uint32_t size, uint32_t offset) override;
    void SetData(const Buffer& buffer, uint32_t offset) override;
    void SetData(Buffer&& buffer, uint32_t offset) override;

    Buffer& GetLocalData() override { return m_Data; }
    void UploadCurrent(uint32_t offset) override;

    void TransferToBuffer(Ref<GraphicsBuffer> other, uint32_t dataSize, uint32_t offset, uint32_t otherOffset) override;

    uint32_t GetSize() const noexcept override { return m_Size; }
protected:
    void Create();
    void Update(uint32_t offset);
protected:
    Buffer m_Data;
    uint32_t m_ID = 0u, m_Size = 0u;
    GraphicsBufferUsage m_Usage;
};

class OpenGLVertexBuffer : public OpenGLGraphicsBuffer, public VertexBuffer {
    friend class OpenGLInputLayout;
public:
    OpenGLVertexBuffer(BufferLayout layout, const void* data, uint32_t size, GraphicsBufferUsage usage)
        : OpenGLGraphicsBuffer(data, size, usage), m_Layout(std::move(layout)) {}
    OpenGLVertexBuffer(BufferLayout layout, const Buffer& buffer, GraphicsBufferUsage usage)
        : OpenGLGraphicsBuffer(buffer, usage), m_Layout(std::move(layout)) {}
    OpenGLVertexBuffer(BufferLayout layout, Buffer&& buffer, GraphicsBufferUsage usage)
        : OpenGLGraphicsBuffer(std::move(buffer), usage), m_Layout(std::move(layout)) {}
    ~OpenGLVertexBuffer() = default;

    const BufferLayout& GetLayout() const noexcept override { return m_Layout; }
private:
    BufferLayout m_Layout;
};

class OpenGLIndexBuffer : public OpenGLGraphicsBuffer, public IndexBuffer {
    friend class OpenGLInputLayout;
public:
    OpenGLIndexBuffer(const void* data, uint32_t size, GraphicsBufferUsage usage)
        : OpenGLGraphicsBuffer(data, size, usage) {}
    OpenGLIndexBuffer(const Buffer& buffer, GraphicsBufferUsage usage)
        : OpenGLGraphicsBuffer(buffer, usage) {}
    OpenGLIndexBuffer(Buffer&& buffer, GraphicsBufferUsage usage)
        : OpenGLGraphicsBuffer(std::move(buffer), usage) {}
    ~OpenGLIndexBuffer() = default;

    uint32_t GetIndicesCount() const noexcept override { return m_Size / sizeof(uint32_t); }
};

class OpenGLConstantBuffer : public OpenGLGraphicsBuffer, public ConstantBuffer {
public:
    OpenGLConstantBuffer([[maybe_unused]] BufferShaderBinding binding, const void* data, uint32_t size)
        : OpenGLGraphicsBuffer(data, size, GraphicsBufferUsage::Default) {}
    OpenGLConstantBuffer([[maybe_unused]] BufferShaderBinding binding, const Buffer& buffer)
        : OpenGLGraphicsBuffer(buffer, GraphicsBufferUsage::Default) {}
    OpenGLConstantBuffer([[maybe_unused]] BufferShaderBinding binding, Buffer&& buffer)
        : OpenGLGraphicsBuffer(std::move(buffer), GraphicsBufferUsage::Default) {}
    OpenGLConstantBuffer([[maybe_unused]] BufferShaderBinding binding, const UniformBufferBase& buffer)
        : OpenGLGraphicsBuffer(buffer.GetBuffer(), buffer.GetBufferSize(), GraphicsBufferUsage::Default) {}
    ~OpenGLConstantBuffer() = default;

    void SetData(const void* data, uint32_t size, uint32_t offset) override;
    void SetData(const Buffer& buffer, uint32_t offset) override;
    void SetData(Buffer&& buffer, uint32_t offset) override;
    void SetData(const UniformBufferBase& buffer) override;

    void Bind(uint32_t slot) const override;
};

} //namespace Saba
