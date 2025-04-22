#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Renderer/GraphicsBuffers.h"
#include "Saba/Renderer/Shader.h"

namespace Saba {

class InputLayout : public RefCounted {
public:
    virtual ~InputLayout() = default;

    virtual Ref<VertexBuffer> GetVertexBuffer(uint32_t index = 0) = 0;
    virtual Ref<IndexBuffer> GetIndexBuffer() noexcept = 0;

    virtual void Bind() const noexcept = 0;

    SB_CORE static Ref<InputLayout> Create(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers,
        const Ref<Shader>& shader = {}, const Ref<IndexBuffer>& indexBuffer = {});
};

} //namespace Saba
