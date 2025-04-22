#pragma once

#include "Saba/Renderer/Texture.h"

namespace Saba {

class Material : public RefCounted {
public:
    Material() = default;
    Material(Ref<Texture2D> colorTexture, Ref<Texture2D> normalTexture = {},
        Ref<Texture2D> metallicTexture = {}, Ref<Texture2D> roughnessTexture = {})
        : m_Color(std::move(colorTexture)), m_Normal(std::move(normalTexture)),
          m_Metallic(std::move(metallicTexture)), m_Roughness(std::move(roughnessTexture)) {}

    Ref<Texture2D> GetColorTexture() const noexcept { return m_Color; }
    Ref<Texture2D> GetNormalTexture() const noexcept { return m_Normal; }
    Ref<Texture2D> GetMetallicTexture() const noexcept { return m_Metallic; }
    Ref<Texture2D> GetRoughnessTexture() const noexcept { return m_Roughness; }
private:
    Ref<Texture2D> m_Color;
    Ref<Texture2D> m_Normal;
    Ref<Texture2D> m_Metallic;
    Ref<Texture2D> m_Roughness;
};

} //namespace Saba
