#pragma once

#include "Saba/Renderer/InputLayout.h"
#include "Saba/Platform/DX11/DX11GraphicsBuffers.h"
#include "Saba/Platform/DX11/DX11Shader.h"

#include <d3d11.h>

namespace Saba {

class DX11InputLayout : public InputLayout
{
public:
    SB_CORE DX11InputLayout(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers, const Ref<Shader>& shader, const Ref<IndexBuffer>& indexBuffer);
    SB_CORE ~DX11InputLayout();

    SB_CORE Ref<VertexBuffer> GetVertexBuffer(uint32_t index) const noexcept override { return m_VertexBuffers[index]; }
    SB_CORE Ref<IndexBuffer> GetIndexBuffer() const noexcept override { return m_IndexBuffer; }

    SB_CORE void Bind() const noexcept override;
private:
    SB_CORE void Create();
private:
    std::vector<Ref<DX11VertexBuffer>> m_VertexBuffers;
    Ref<DX11Shader> m_Shader;
    Ref<DX11IndexBuffer> m_IndexBuffer;
    ComPtr<ID3D11InputLayout> m_InputLayout;
    
    uint32_t* m_Strides;
    ID3D11Buffer** m_Buffers;
};

}
