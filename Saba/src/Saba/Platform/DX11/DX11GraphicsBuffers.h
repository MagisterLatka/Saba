#pragma once

#include "Saba/Renderer/GraphicsBuffers.h"

#include <d3d11.h>

namespace Saba {

class DX11VertexBuffer : public VertexBuffer {
    friend class DX11InputLayout;
public:
    SB_CORE DX11VertexBuffer(BufferLayout layout, const void* data, uint32_t size, BufferUsage usage);
    SB_CORE DX11VertexBuffer(BufferLayout layout, const Buffer& buffer, BufferUsage usage);
    SB_CORE DX11VertexBuffer(BufferLayout layout, Buffer&& buffer, BufferUsage usage);
    SB_CORE ~DX11VertexBuffer() = default;

    SB_CORE void SetData(const void* data, uint32_t size, uint32_t offset = 0u) override;
    SB_CORE void SetData(const Buffer& buffer, uint32_t offset = 0u) override;
    SB_CORE void SetData(Buffer&& buffer, uint32_t offset = 0u) override;

    SB_CORE Buffer& GetLocalData() override { return m_Data; }
    SB_CORE void UploadCurrent(uint32_t offset = 0u) override;

    SB_CORE const BufferLayout& GetLayout() const noexcept override { return m_Layout; }
    SB_CORE uint32_t GetSize() const noexcept override { return m_Size; }
private:
    SB_CORE void Create();
    SB_CORE void Update(uint32_t offset);
private:
    Buffer m_Data;
    BufferLayout m_Layout;
    ComPtr<ID3D11Buffer> m_Buffer;
    uint32_t m_Size = 0u;
    BufferUsage m_Usage;
};

class DX11IndexBuffer : public IndexBuffer {
    friend class DX11InputLayout;
public:
    SB_CORE DX11IndexBuffer(const uint32_t* data, uint32_t size, BufferUsage usage);
    SB_CORE DX11IndexBuffer(const Buffer& buffer, BufferUsage usage);
    SB_CORE DX11IndexBuffer(Buffer&& buffer, BufferUsage usage);
    SB_CORE virtual ~DX11IndexBuffer() = default;

    SB_CORE void SetData(const uint32_t* data, uint32_t size, uint32_t offset) override;
    SB_CORE void SetData(const Buffer& buffer, uint32_t offset) override;
    SB_CORE void SetData(Buffer&& buffer, uint32_t offset) override;

    SB_CORE uint32_t GetSize() const noexcept override { return m_Size; }
    SB_CORE uint32_t GetIndicesCount() const noexcept override { return m_Size / sizeof(uint32_t); }
private:
    SB_CORE void Create();
    SB_CORE void Update(uint32_t offset);
private:
    Buffer m_Data;
    ComPtr<ID3D11Buffer> m_Buffer;
    uint32_t m_Size = 0u;
    BufferUsage m_Usage;
};

class DX11ConstantBuffer : public ConstantBuffer {
public:
    SB_CORE DX11ConstantBuffer(BufferShaderBinding binding, const void* data, uint32_t size);
    SB_CORE DX11ConstantBuffer(BufferShaderBinding binding, const Buffer& buffer);
    SB_CORE DX11ConstantBuffer(BufferShaderBinding binding, Buffer&& buffer);
    SB_CORE DX11ConstantBuffer(BufferShaderBinding binding, const UniformBufferBase& buffer);
    SB_CORE virtual ~DX11ConstantBuffer() = default;

    SB_CORE void SetData(const void* data, uint32_t size) override;
    SB_CORE void SetData(const Buffer& buffer) override;
    SB_CORE void SetData(Buffer&& buffer) override;
    SB_CORE void SetData(const UniformBufferBase& buffer) override;

    SB_CORE Buffer& GetLocalData() override { return m_Data; }
    SB_CORE void UploadCurrent() override;

    SB_CORE void Bind(uint32_t slot) const override;

    SB_CORE uint32_t GetSize() const noexcept override { return m_Size; }
private:
    SB_CORE void Create();
    SB_CORE void Update();
private:
    Buffer m_Data;
    ComPtr<ID3D11Buffer> m_Buffer;
    uint32_t m_Size = 0u;
    BufferShaderBinding m_Binding;
};

}
