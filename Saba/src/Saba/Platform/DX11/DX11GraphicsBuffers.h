#pragma once

#include "Saba/Renderer/GraphicsBuffers.h"

#include <d3d11.h>

namespace Saba {

class DX11GraphicsBuffer : virtual public GraphicsBuffer {
public:
    DX11GraphicsBuffer(const void* data, uint32_t size, GraphicsBufferUsage usage);
    DX11GraphicsBuffer(const Buffer& buffer, GraphicsBufferUsage usage);
    DX11GraphicsBuffer(Buffer&& buffer, GraphicsBufferUsage usage);
    virtual ~DX11GraphicsBuffer() = default;

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
    ComPtr<ID3D11Buffer> m_Buffer;
    uint32_t m_Size = 0u;
    GraphicsBufferUsage m_Usage;
};

class DX11VertexBuffer : public DX11GraphicsBuffer, public VertexBuffer {
    friend class DX11GraphicsBuffer;
    friend class DX11InputLayout;
public:
    DX11VertexBuffer(BufferLayout layout, const void* data, uint32_t size, GraphicsBufferUsage usage)
        : DX11GraphicsBuffer(data, size, usage), m_Layout(std::move(layout)) {}
    DX11VertexBuffer(BufferLayout layout, const Buffer& buffer, GraphicsBufferUsage usage)
        : DX11GraphicsBuffer(buffer, usage), m_Layout(std::move(layout)) {}
    DX11VertexBuffer(BufferLayout layout, Buffer&& buffer, GraphicsBufferUsage usage)
        : DX11GraphicsBuffer(std::move(buffer), usage), m_Layout(std::move(layout)) {}
    ~DX11VertexBuffer() = default;

     const BufferLayout& GetLayout() const noexcept override { return m_Layout; }
private:
    BufferLayout m_Layout;
};

class DX11IndexBuffer : public DX11GraphicsBuffer, public IndexBuffer {
    friend class DX11InputLayout;
public:
    DX11IndexBuffer(const void* data, uint32_t size, GraphicsBufferUsage usage)
        : DX11GraphicsBuffer(data, size, usage) {}
    DX11IndexBuffer(const Buffer& buffer, GraphicsBufferUsage usage)
        : DX11GraphicsBuffer(buffer, usage) {}
    DX11IndexBuffer(Buffer&& buffer, GraphicsBufferUsage usage)
        : DX11GraphicsBuffer(std::move(buffer), usage) {}
    ~DX11IndexBuffer() = default;

    SB_CORE uint32_t GetIndicesCount() const noexcept override { return m_Size / sizeof(uint32_t); }
};

class DX11ConstantBuffer : public DX11GraphicsBuffer, public ConstantBuffer {
public:
    DX11ConstantBuffer(BufferShaderBinding binding, const void* data, uint32_t size)
        : DX11GraphicsBuffer(data, size, GraphicsBufferUsage::Default), m_Binding(binding) {}
    DX11ConstantBuffer(BufferShaderBinding binding, const Buffer& buffer)
        : DX11GraphicsBuffer(buffer, GraphicsBufferUsage::Default), m_Binding(binding) {}
    DX11ConstantBuffer(BufferShaderBinding binding, Buffer&& buffer)
        : DX11GraphicsBuffer(std::move(buffer), GraphicsBufferUsage::Default), m_Binding(binding) {}
    DX11ConstantBuffer(BufferShaderBinding binding, const UniformBufferBase& buffer)
        : DX11GraphicsBuffer(buffer.GetBuffer(), buffer.GetBufferSize(), GraphicsBufferUsage::Default), m_Binding(binding) {}
    ~DX11ConstantBuffer() = default;

    void SetData(const void* data, uint32_t size, uint32_t offset) override;
    void SetData(const Buffer& buffer, uint32_t offset) override;
    void SetData(Buffer&& buffer, uint32_t offset) override;
    void SetData(const UniformBufferBase& buffer) override;

    void Bind(uint32_t slot) const override;
private:
    BufferShaderBinding m_Binding;
};

}
