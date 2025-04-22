#include <pch.h>
#include "DX11GraphicsBuffers.h"

#include "Saba/Renderer/Renderer.h"
#include "Saba/Platform/DX11/DX11Context.h"

namespace Saba {

static D3D11_USAGE GetUsage(GraphicsBufferUsage usage) {
    switch (usage) {
        case GraphicsBufferUsage::Default:   return D3D11_USAGE_DEFAULT;
        case GraphicsBufferUsage::Dynamic:   return D3D11_USAGE_DYNAMIC;
        case GraphicsBufferUsage::Immutable: return D3D11_USAGE_IMMUTABLE;
        case GraphicsBufferUsage::Staging:   return D3D11_USAGE_STAGING;
        default: break;
    }
    SB_CORE_THROW_INFO("Unknown buffer usage");
    return D3D11_USAGE_DEFAULT;
}

DX11GraphicsBuffer::DX11GraphicsBuffer(const void* data, uint32_t size, GraphicsBufferUsage usage)
    : m_Size(size), m_Usage(usage)
{
    if (data != nullptr)
        m_Data = Buffer::Copy(data, size);
    Create();
}
DX11GraphicsBuffer::DX11GraphicsBuffer(const Buffer& buffer, GraphicsBufferUsage usage)
    : m_Data(buffer), m_Size(buffer.Size), m_Usage(usage)
{
    Create();
}
DX11GraphicsBuffer::DX11GraphicsBuffer(Buffer&& buffer, GraphicsBufferUsage usage)
    : m_Data(std::move(buffer)), m_Size(m_Data.Size), m_Usage(usage)
{
    Create();
}
void DX11GraphicsBuffer::Create() {
    SB_CORE_ASSERT(m_Usage > GraphicsBufferUsage::None && m_Usage <= GraphicsBufferUsage::Last,
        "Invalid graphics buffer usage: {0}", static_cast<uint32_t>(m_Usage));

    Ref<DX11GraphicsBuffer> instance = this;
    Renderer::Submit([instance]() mutable {
        HRESULT hr;

        D3D11_BUFFER_DESC bufferDesc = { 0 };
        bufferDesc.ByteWidth = instance->m_Size;
        bufferDesc.Usage = GetUsage(instance->m_Usage);
        if (instance->m_Usage == GraphicsBufferUsage::Dynamic || instance->m_Usage == GraphicsBufferUsage::Staging)
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0u;

        if (instance.CanConvert<DX11VertexBuffer>()) {
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.StructureByteStride = instance.As<DX11VertexBuffer>()->m_Layout.GetStride();
        }
        else if (instance.CanConvert<DX11IndexBuffer>()) {
            bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        }
        else {
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        }
        if (instance->m_Usage == GraphicsBufferUsage::Staging)
            bufferDesc.BindFlags = 0u;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = instance->m_Data.Data;
        SB_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateBuffer(&bufferDesc, instance->m_Data.Data ? &data : nullptr,
            &instance->m_Buffer));
    });
}

void DX11GraphicsBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != GraphicsBufferUsage::Immutable, "Cannot modify immutable graphics buffer");
    SB_CORE_ASSERT(size + offset <= m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        size, offset, m_Size);
    m_Data = Buffer::Copy(data, size);
    Update(offset);
}
void DX11GraphicsBuffer::SetData(const Buffer& buffer, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != GraphicsBufferUsage::Immutable, "Cannot modify immutable graphics buffer");
    SB_CORE_ASSERT(m_Data.Size + offset <= m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        m_Data.Size, offset, m_Size);
    m_Data = buffer;
    Update(offset);
}
void DX11GraphicsBuffer::SetData(Buffer&& buffer, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != GraphicsBufferUsage::Immutable, "Cannot modify immutable graphics buffer");
    SB_CORE_ASSERT(m_Data.Size + offset <= m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        m_Data.Size, offset, m_Size);
    m_Data = std::move(buffer);
    Update(offset);
}
void DX11GraphicsBuffer::UploadCurrent(uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != GraphicsBufferUsage::Immutable, "Cannot modify immutable graphics buffer");
    SB_CORE_ASSERT(m_Data.Size + offset <= m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        m_Data.Size, offset, m_Size);
    Update(offset);
}
void DX11GraphicsBuffer::Update(uint32_t offset) {
    Ref<DX11GraphicsBuffer> instance = this;
    Renderer::Submit([instance, offset]() {
        auto context = DX11Context::GetContextFromApplication()->GetContext();
        if (instance->m_Usage == GraphicsBufferUsage::Dynamic || instance->m_Usage == GraphicsBufferUsage::Staging) {
            D3D11_MAPPED_SUBRESOURCE map;
            HRESULT hr;
            SB_DX_GRAPHICS_CALL_INFO(context->Map(instance->m_Buffer.Get(), 0u,
                instance->m_Usage == GraphicsBufferUsage::Dynamic ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE, 0u, &map));
            memcpy((uint8_t*)map.pData + offset, instance->m_Data.Data, instance->m_Data.Size);
            context->Unmap(instance->m_Buffer.Get(), 0u);
        }
        else {
            D3D11_BOX box = { 0 };
            box.left = offset;
            box.right = offset + instance->m_Data.Size;
            box.bottom = 1u;
            box.back = 1u;
            D3D11_BOX* boxPtr = &box;
            if (instance.CanConvert<DX11ConstantBuffer>()) {
                boxPtr = nullptr;
            }
            SB_DX_GRAPHICS_CALL_INFO_ONLY(context->UpdateSubresource(instance->m_Buffer.Get(), 0u, boxPtr, instance->m_Data.Data, 0u, 0u));
        }
    });
}

void DX11GraphicsBuffer::TransferToBuffer(Ref<GraphicsBuffer> other, uint32_t dataSize, uint32_t offset, uint32_t otherOffset) {
    SB_CORE_ASSERT(other.CanConvert<DX11GraphicsBuffer>(), "Using TransferToBuffer function is only available between same RendererAPIs");
    auto second = other.As<DX11GraphicsBuffer>();
    SB_CORE_ASSERT(offset + dataSize <= m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        dataSize, offset, m_Size);
    SB_CORE_ASSERT(otherOffset + dataSize <= second->m_Size, "Graphics buffer overflow (size: {0}, offset: {1}, maxSize: {2})",
        dataSize, otherOffset, second->m_Size);

    Ref<DX11GraphicsBuffer> instance = this;
    Renderer::Submit([instance, second, dataSize, offset, otherOffset]() {
        auto context = DX11Context::GetContextFromApplication()->GetContext();
        D3D11_BOX box = { 0 };
        box.left = offset;
        box.right = offset + instance->m_Data.Size;
        box.bottom = 1u;
        box.back = 1u;
        context->CopySubresourceRegion(second->m_Buffer.Get(), 0u, otherOffset, 0u, 0u, instance->m_Buffer.Get(), 0u, &box);
    });
}

void DX11ConstantBuffer::SetData(const void* data, uint32_t size, [[maybe_unused]] uint32_t offset) {
    SB_CORE_ASSERT(size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = Buffer::Copy(data, size);
    Update(0u);
}
void DX11ConstantBuffer::SetData(const Buffer& buffer, [[maybe_unused]] uint32_t offset) {
    SB_CORE_ASSERT(buffer.Size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = buffer;
    Update(0u);
}
void DX11ConstantBuffer::SetData(Buffer&& buffer, [[maybe_unused]] uint32_t offset) {
    SB_CORE_ASSERT(buffer.Size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = buffer;
    Update(0u);
}
void DX11ConstantBuffer::SetData(const UniformBufferBase& buffer) {
    SB_CORE_ASSERT(buffer.GetBufferSize() == m_Size, "Only updating whole constant buffer is possible");
    m_Data = Buffer::Copy(buffer.GetBuffer(), buffer.GetBufferSize());
    Update(0u);
}

void DX11ConstantBuffer::Bind(uint32_t slot) const {
    Ref<const DX11ConstantBuffer> instance = this;
    Renderer::Submit([instance, slot]() {
        if (instance->m_Binding == BufferShaderBinding::Vertex)
            DX11Context::GetContextFromApplication()->GetContext()->VSSetConstantBuffers(slot, 1u, instance->m_Buffer.GetAddressOf());
        else if (instance->m_Binding == BufferShaderBinding::Fragment)
            DX11Context::GetContextFromApplication()->GetContext()->PSSetConstantBuffers(slot, 1u, instance->m_Buffer.GetAddressOf());
    });
}

}
