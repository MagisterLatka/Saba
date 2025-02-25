#include <pch.h>
#include "DX11InputLayout.h"

#include "Saba/Renderer/Renderer.h"
#include "Saba/Platform/DX11/DX11Context.h"

namespace Saba {

static DXGI_FORMAT GetFormat(BufferLayoutElementDataType type) {
    switch (type) {
        case BufferLayoutElementDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
        case BufferLayoutElementDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
        case BufferLayoutElementDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
        case BufferLayoutElementDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case BufferLayoutElementDataType::Mat3:     return DXGI_FORMAT_R32G32B32_FLOAT;
        case BufferLayoutElementDataType::Mat4:     return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case BufferLayoutElementDataType::Int:      return DXGI_FORMAT_R32_SINT;
        case BufferLayoutElementDataType::Int2:     return DXGI_FORMAT_R32G32_SINT;
        case BufferLayoutElementDataType::Int3:     return DXGI_FORMAT_R32G32B32_SINT;
        case BufferLayoutElementDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_SINT;
        case BufferLayoutElementDataType::UInt:     return DXGI_FORMAT_R32_UINT;
        case BufferLayoutElementDataType::UInt2:    return DXGI_FORMAT_R32G32_UINT;
        case BufferLayoutElementDataType::UInt3:    return DXGI_FORMAT_R32G32B32_UINT;
        case BufferLayoutElementDataType::UInt4:    return DXGI_FORMAT_R32G32B32A32_UINT;
    }
    SB_CORE_THROW_INFO("Unknown buffer layout element data type");
    return DXGI_FORMAT_UNKNOWN;
}

DX11InputLayout::DX11InputLayout(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers, const Ref<Shader>& shader, const Ref<IndexBuffer>& indexBuffer)
    : m_Shader(std::move(shader)), m_IndexBuffer(std::move(indexBuffer))
{
    for (const auto& buffer : vertexBuffers)
        m_VertexBuffers.push_back(buffer);

    SB_CORE_ASSERT(vertexBuffers.size() > 0, "Input layout requires at least vertex buffer");
    SB_CORE_ASSERT(shader, "Input layout requires shader");
    Create();
}
DX11InputLayout::~DX11InputLayout() {
    if (m_Buffers) delete[] m_Buffers;
    if (m_Strides) delete[] m_Strides;
}
void DX11InputLayout::Create() {
    Ref<DX11InputLayout> instance = this;
    Renderer::Submit([instance]() mutable {
        uint32_t count = 0u;
        for (const auto& buffer : instance->m_VertexBuffers) {
            const auto& elements = buffer->GetLayout().GetElements();
            for (auto& element : elements) {
                if (element.Type == BufferLayoutElementDataType::Mat3)
                    count += 3;
                else if (element.Type == BufferLayoutElementDataType::Mat4)
                    count += 4;
                else count += 1;
            }
        }
        D3D11_INPUT_ELEMENT_DESC* inputElements = new D3D11_INPUT_ELEMENT_DESC[count];
        uint32_t index = 0u;
        instance->m_Strides = new uint32_t[instance->m_VertexBuffers.size()];
        instance->m_Buffers = new ID3D11Buffer*[instance->m_VertexBuffers.size()];
        for (int i = 0; i < instance->m_VertexBuffers.size(); ++i) {
            instance->m_Strides[i] = instance->m_VertexBuffers[i]->m_Layout.GetStride();
            instance->m_Buffers[i] = instance->m_VertexBuffers[i]->m_Buffer.Get();

            auto& elements = instance->m_VertexBuffers[i]->GetLayout().GetElements();
            for (const auto& element : elements) {
                if (element.Type == BufferLayoutElementDataType::Mat3) {
                    for (uint32_t j = 0; j < 3; ++j) {
                        inputElements[index].SemanticName = element.Name.c_str();
                        inputElements[index].SemanticIndex = j;
                        inputElements[index].Format = GetFormat(element.Type);
                        inputElements[index].InputSlot = i;
                        inputElements[index].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                        inputElements[index].InputSlotClass = element.InstanceStepRate == 0 ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
                        inputElements[index].InstanceDataStepRate = element.InstanceStepRate;
                        ++index;
                    }
                }
                else if (element.Type == BufferLayoutElementDataType::Mat4) {
                    for (uint32_t j = 0; j < 4; ++j) {
                        inputElements[index].SemanticName = element.Name.c_str();
                        inputElements[index].SemanticIndex = j;
                        inputElements[index].Format = GetFormat(element.Type);
                        inputElements[index].InputSlot = i;
                        inputElements[index].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                        inputElements[index].InputSlotClass = element.InstanceStepRate == 0 ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
                        inputElements[index].InstanceDataStepRate = element.InstanceStepRate;
                        ++index;
                    }
                }
                else {
                    inputElements[index].SemanticName = element.Name.c_str();
                    inputElements[index].SemanticIndex = 0u;
                    inputElements[index].Format = GetFormat(element.Type);
                    inputElements[index].InputSlot = i;
                    inputElements[index].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                    inputElements[index].InputSlotClass = element.InstanceStepRate == 0 ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
                    inputElements[index].InstanceDataStepRate = element.InstanceStepRate;
                    ++index;
                }
            }
        }

        HRESULT hr;
        SB_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateInputLayout(inputElements, count,
            instance->m_Shader->GetVertexSource()->GetBufferPointer(), instance->m_Shader->GetVertexSource()->GetBufferSize(), &instance->m_InputLayout));
        delete[] inputElements;
    });
}

void DX11InputLayout::Bind() const noexcept {
    Ref<const DX11InputLayout> instance = this;
    Renderer::Submit([instance]() {
        auto context = DX11Context::GetContextFromApplication()->GetContext();
        static uint32_t offsets[16] = { 0 };
        context->IASetVertexBuffers(0u, (uint32_t)instance->m_VertexBuffers.size(), instance->m_Buffers, instance->m_Strides, offsets);
        if (instance->m_IndexBuffer) context->IASetIndexBuffer(instance->m_IndexBuffer->m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
        context->IASetInputLayout(instance->m_InputLayout.Get());
    });
}

}
