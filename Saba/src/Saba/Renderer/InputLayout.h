#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Renderer/GraphicsBuffers.h"
#include "Saba/Renderer/Shader.h"

namespace Saba {

class InputLayout : public RefCounted {
public:
    SB_CORE virtual ~InputLayout() = default;

    SB_CORE virtual Ref<VertexBuffer> GetVertexBuffer(uint32_t index = 0) const = 0;
    SB_CORE virtual Ref<IndexBuffer> GetIndexBuffer() const noexcept = 0;

    SB_CORE virtual void Bind() const noexcept = 0;

    SB_CORE static Ref<InputLayout> Create(const std::initializer_list<Ref<VertexBuffer>>& vertexBuffers,
        const Ref<Shader>& shader = {}, const Ref<IndexBuffer>& indexBuffer = {});
};

}
