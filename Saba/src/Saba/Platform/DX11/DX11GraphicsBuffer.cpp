#include <pch.h>
#include "DX11GraphicsBuffers.h"

#include "Saba/Renderer/Renderer.h"
#include "Saba/Platform/DX11/DX11Context.h"

namespace Saba {

static D3D11_USAGE GetUsage(BufferUsage usage) {
    switch (usage) {
        case Saba::BufferUsage::Default:   return D3D11_USAGE_DEFAULT;
        case Saba::BufferUsage::Dynamic:   return D3D11_USAGE_DYNAMIC;
        case Saba::BufferUsage::Immutable: return D3D11_USAGE_IMMUTABLE;
        default: break;
    }
    SB_CORE_THROW_INFO("Unknown buffer usage");
    return D3D11_USAGE_DEFAULT;
}

DX11VertexBuffer::DX11VertexBuffer(BufferLayout layout, void* data, uint32_t size, BufferUsage usage)
    : m_Layout(std::move(layout)), m_Size(size), m_Usage(usage)
{
    if (data != nullptr)
        m_Data = Buffer::Copy(data, size);
    Create();
}
DX11VertexBuffer::DX11VertexBuffer(BufferLayout layout, const Buffer& buffer, BufferUsage usage)
    : m_Data(buffer), m_Layout(std::move(layout)), m_Size(buffer.Size), m_Usage(usage)
{
    Create();
}
DX11VertexBuffer::DX11VertexBuffer(BufferLayout layout, Buffer&& buffer, BufferUsage usage)
    : m_Data(std::move(buffer)), m_Layout(std::move(layout)), m_Size(buffer.Size), m_Usage(usage)
{
    Create();
}
void DX11VertexBuffer::Create() {
    Ref<DX11VertexBuffer> instance = this;
    Renderer::Submit([instance]() mutable {
        HRESULT hr;

        D3D11_BUFFER_DESC bufferDesc = { 0 };
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.ByteWidth = instance->m_Size;
        bufferDesc.CPUAccessFlags = instance->m_Usage == BufferUsage::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.StructureByteStride = instance->m_Layout.GetStride();
        bufferDesc.Usage = GetUsage(instance->m_Usage);
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = instance->m_Data.Data;
        SB_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateBuffer(&bufferDesc, &data, &instance->m_Buffer));
    });
}

void DX11VertexBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    if (m_Usage == BufferUsage::Dynamic) SB_CORE_ASSERT(size == m_Size && offset == 0u, "Dynamic buffer cannot be updated partially");
    SB_CORE_ASSERT(size + offset <= m_Size, "Vertex buffer overflow");
    m_Data = Buffer::Copy(data, size);
    Update(offset);
}
void DX11VertexBuffer::SetData(const Buffer& buffer, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    if (m_Usage == BufferUsage::Dynamic) SB_CORE_ASSERT(buffer.Size == m_Size && offset == 0u, "Dynamic buffer cannot be updated partially");
    SB_CORE_ASSERT(buffer.Size + offset <= m_Size, "Vertex buffer overflow");
    m_Data = buffer;
    Update(offset);
}
void DX11VertexBuffer::SetData(Buffer&& buffer, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    if (m_Usage == BufferUsage::Dynamic) SB_CORE_ASSERT(buffer.Size == m_Size && offset == 0u, "Dynamic buffer cannot be updated partially");
    SB_CORE_ASSERT(buffer.Size + offset <= m_Size, "Vertex buffer overflow");
    m_Data = std::move(buffer);
    Update(offset);
}
void DX11VertexBuffer::Update(uint32_t offset) {
    Ref<DX11VertexBuffer> instance = this;
    Renderer::Submit([instance, offset]() {
        auto context = DX11Context::GetContextFromApplication()->GetContext();
        if (instance->m_Usage == BufferUsage::Dynamic) {
            D3D11_MAPPED_SUBRESOURCE map;
            HRESULT hr;
            SB_DX_GRAPHICS_CALL_INFO(context->Map(instance->m_Buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &map));
            memcpy((uint8_t*)map.pData + offset, instance->m_Data.Data, instance->m_Data.Size);
            context->Unmap(instance->m_Buffer.Get(), 0u);
        }
        else {
            D3D11_BOX box = { 0 };
            box.left = offset;
            box.right = offset + instance->m_Data.Size;
            box.bottom = 1u;
            box.back = 1u;
            SB_DX_GRAPHICS_CALL_INFO_ONLY(context->UpdateSubresource(instance->m_Buffer.Get(), 0u, &box, instance->m_Data.Data, 0u, 0u));
        }
    });
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DX11IndexBuffer::DX11IndexBuffer(uint32_t* data, uint32_t size, BufferUsage usage)
    : m_Size(size), m_Usage(usage)
{
    if (data != nullptr)
        m_Data = Buffer::Copy(data, size);
    Create();
}
DX11IndexBuffer::DX11IndexBuffer(const Buffer& buffer, BufferUsage usage)
    : m_Data(buffer), m_Size(buffer.Size), m_Usage(usage)
{
    Create();
}
DX11IndexBuffer::DX11IndexBuffer(Buffer&& buffer, BufferUsage usage)
    : m_Data(std::move(buffer)), m_Size(buffer.Size), m_Usage(usage)
{
    Create();
}
void DX11IndexBuffer::Create() {
    Ref<DX11IndexBuffer> instance = this;
    Renderer::Submit([instance]() mutable {
        HRESULT hr;

        D3D11_BUFFER_DESC bufferDesc = { 0 };
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.ByteWidth = instance->m_Size;
        bufferDesc.CPUAccessFlags = instance->m_Usage == BufferUsage::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.StructureByteStride = 0u;
        bufferDesc.Usage = GetUsage(instance->m_Usage);
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = instance->m_Data.Data;
        SB_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateBuffer(&bufferDesc, &data, &instance->m_Buffer));
    });
}

void DX11IndexBuffer::SetData(uint32_t* data, uint32_t size, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    if (m_Usage == BufferUsage::Dynamic) SB_CORE_ASSERT(size == m_Size && offset == 0u, "Dynamic buffer cannot be updated partially");
    SB_CORE_ASSERT(size + offset <= m_Size, "Vertex buffer overflow");
    m_Data = Buffer::Copy(data, size);
    Update(offset);
}
void DX11IndexBuffer::SetData(const Buffer& buffer, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    if (m_Usage == BufferUsage::Dynamic) SB_CORE_ASSERT(buffer.Size == m_Size && offset == 0u, "Dynamic buffer cannot be updated partially");
    SB_CORE_ASSERT(buffer.Size + offset <= m_Size, "Vertex buffer overflow");
    m_Data = buffer;
    Update(offset);
}
void DX11IndexBuffer::SetData(Buffer&& buffer, uint32_t offset) {
    SB_CORE_ASSERT(m_Usage != BufferUsage::Immutable, "Cannot modify immutable buffer");
    if (m_Usage == BufferUsage::Dynamic) SB_CORE_ASSERT(buffer.Size == m_Size && offset == 0u, "Dynamic buffer cannot be updated partially");
    SB_CORE_ASSERT(buffer.Size + offset <= m_Size, "Vertex buffer overflow");
    m_Data = std::move(buffer);
    Update(offset);
}
void DX11IndexBuffer::Update(uint32_t offset) {
    Ref<DX11IndexBuffer> instance = this;
    Renderer::Submit([instance, offset]() {
        auto context = DX11Context::GetContextFromApplication()->GetContext();
        if (instance->m_Usage == BufferUsage::Dynamic) {
            D3D11_MAPPED_SUBRESOURCE map;
            HRESULT hr;
            SB_DX_GRAPHICS_CALL_INFO(context->Map(instance->m_Buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &map));
            memcpy(map.pData, instance->m_Data.Data, instance->m_Data.Size);
            context->Unmap(instance->m_Buffer.Get(), 0u);
        }
        else {
            D3D11_BOX box = { 0 };
            box.left = offset;
            box.right = offset + instance->m_Data.Size;
            box.bottom = 1u;
            box.back = 1u;
            SB_DX_GRAPHICS_CALL_INFO_ONLY(context->UpdateSubresource(instance->m_Buffer.Get(), 0u, &box, instance->m_Data.Data, 0u, 0u));
        }
    });
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


DX11ConstantBuffer::DX11ConstantBuffer(BufferShaderBinding binding, void* data, uint32_t size)
    : m_Size(size), m_Binding(binding)
{
    if (data != nullptr)
        m_Data = Buffer::Copy(data, size);
    Create();
}
DX11ConstantBuffer::DX11ConstantBuffer(BufferShaderBinding binding, const Buffer& buffer)
    : m_Data(buffer), m_Size(buffer.Size), m_Binding(binding)
{
    Create();
}
DX11ConstantBuffer::DX11ConstantBuffer(BufferShaderBinding binding, Buffer&& buffer)
    : m_Data(std::move(buffer)), m_Size(buffer.Size), m_Binding(binding)
{
    Create();
}
DX11ConstantBuffer::DX11ConstantBuffer(BufferShaderBinding binding, const UniformBufferBase& buffer)
    : m_Data(Buffer::Copy(buffer.GetBuffer(), buffer.GetBufferSize())), m_Size(buffer.GetBufferSize()), m_Binding(binding)
{
    Create();
}
void DX11ConstantBuffer::Create() {
    Ref<DX11ConstantBuffer> instance = this;
    Renderer::Submit([instance]() mutable {
        HRESULT hr;

        D3D11_BUFFER_DESC bufferDesc = { 0 };
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.ByteWidth = instance->m_Size;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.StructureByteStride = 0u;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = instance->m_Data.Data;
        SB_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateBuffer(&bufferDesc, &data, &instance->m_Buffer));
    });
}

void DX11ConstantBuffer::SetData(void* data, uint32_t size) {
    SB_CORE_ASSERT(size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = Buffer::Copy(data, size);
    Update();
}
void DX11ConstantBuffer::SetData(const Buffer& buffer) {
    SB_CORE_ASSERT(buffer.Size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = buffer;
    Update();
}
void DX11ConstantBuffer::SetData(Buffer&& buffer) {
    SB_CORE_ASSERT(buffer.Size == m_Size, "Only updating whole constant buffer is possible");
    m_Data = buffer;
    Update();
}
void DX11ConstantBuffer::SetData(const UniformBufferBase& buffer) {
    SB_CORE_ASSERT(buffer.GetBufferSize() == m_Size, "Only updating whole constant buffer is possible");
    m_Data = Buffer::Copy(buffer.GetBuffer(), buffer.GetBufferSize());
    Update();
}
void DX11ConstantBuffer::Update() {
    Ref<DX11ConstantBuffer> instance = this;
    Renderer::Submit([instance]() mutable {
        SB_DX_GRAPHICS_CALL_INFO_ONLY(DX11Context::GetContextFromApplication()->GetContext()->UpdateSubresource(instance->m_Buffer.Get(), 0u, nullptr,
            instance->m_Data.Data, 0u, 0u));
    });
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
