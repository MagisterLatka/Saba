#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Renderer/Texture.h"

namespace Saba {

class Renderer2D
{
    friend class Renderer;
public:
    struct Stats {
        uint32_t QuadCount = 0u, DrawCalls = 0u;
    };
public:
    SB_CORE static void SetViewProjectionMatrix(const glm::mat4& viewProjMat);

    SB_CORE static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, float rotation = 0.0f, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
        const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f);
    SB_CORE static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, float rotation = 0.0f, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
        const Ref<Texture2D>& texture = {}, float tillingFactor = 1.0f);

    SB_CORE static void Draw();

    SB_CORE static Stats GetStats() noexcept;
    SB_CORE static void ResetStats() noexcept;
private:
    SB_CORE static void Init();
    SB_CORE static void Shutdown();
};

}
