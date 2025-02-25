#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class Shader : public RefCounted {
public:
    using ShaderReloadedCallback = std::function<void()>;

    SB_CORE virtual ~Shader() = default;

    SB_CORE virtual void Reload() = 0;
    SB_CORE virtual void Reload(std::string vertexSource, std::string fragmentSource) = 0;
    SB_CORE virtual void Bind() const noexcept = 0;

    SB_CORE virtual const std::string& GetName() const noexcept = 0;
    SB_CORE virtual void AddReloadedCallback(ShaderReloadedCallback callback) noexcept = 0;

    SB_CORE static Ref<Shader> Create(std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
    SB_CORE static Ref<Shader> Create(std::filesystem::path path);
    SB_CORE static Ref<Shader> Create(std::string vertexSource, std::string fragmentSource, std::string debugName = {});
};

class ShaderLibrary : public RefCounted {
public:
    SB_CORE ShaderLibrary() = default;
    SB_CORE ~ShaderLibrary() = default;

    SB_CORE void Add(Ref<Shader> shader);
    SB_CORE void Add(const std::string& name, Ref<Shader> shader);

    SB_CORE void Load(std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
    SB_CORE void Load(std::filesystem::path path);
    SB_CORE void Load(const std::string& name, std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
    SB_CORE void Load(const std::string& name, std::filesystem::path path);
    SB_CORE void Load(const std::string& name, std::string vertexSource, std::string fragmentSource);

    SB_CORE Ref<Shader> Get(const std::string& name) const;
private:
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};

}
