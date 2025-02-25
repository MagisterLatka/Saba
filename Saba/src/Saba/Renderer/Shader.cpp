#include <pch.h>
#include "Shader.h"

#include "Saba/Renderer/RendererAPI.h"
#include "Saba/Platform/OpenGL/OpenGLShader.h"
#if defined(SB_PLATFORM_WINDOWS)
#   include "Saba/Platform/DX11/DX11Shader.h"
#endif

namespace Saba {

Ref<Shader> Shader::Create(std::filesystem::path vertexPath, std::filesystem::path fragmentPath) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLShader>::Create(std::move(vertexPath), std::move(fragmentPath));
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11Shader>::Create(std::move(vertexPath), std::move(fragmentPath));
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}
Ref<Shader> Shader::Create(std::filesystem::path path) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLShader>::Create(std::move(path));
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11Shader>::Create(std::move(path));
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}
Ref<Shader> Shader::Create(std::string vertexSource, std::string fragmentSource, std::string debugName) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLShader>::Create(std::move(vertexSource), std::move(fragmentSource), std::move(debugName));
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11Shader>::Create(std::move(vertexSource), std::move(fragmentSource), std::move(debugName));
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}

void ShaderLibrary::Add(Ref<Shader> shader) {
    const auto& name = shader->GetName();
    SB_CORE_ASSERT(m_Shaders.contains(name), "Shader {0} already exists", name);
    m_Shaders.emplace(name, std::move(shader));
}
void ShaderLibrary::Add(const std::string& name, Ref<Shader> shader) {
    SB_CORE_ASSERT(m_Shaders.contains(name), "Shader {0} already exists", name);
    m_Shaders.emplace(name, std::move(shader));
}

void ShaderLibrary::Load(std::filesystem::path vertexPath, std::filesystem::path fragmentPath) {
    Ref<Shader> shader = Shader::Create(std::move(vertexPath), std::move(fragmentPath));
    const auto& name = shader->GetName();
    SB_CORE_ASSERT(m_Shaders.contains(name), "Shader {0} already exists", name);
    m_Shaders.emplace(name, std::move(shader));
}
void ShaderLibrary::Load(std::filesystem::path path) {
    Ref<Shader> shader = Shader::Create(std::move(path));
    const auto& name = shader->GetName();
    SB_CORE_ASSERT(m_Shaders.contains(name), "Shader {0} already exists", name);
    m_Shaders.emplace(name, std::move(shader));
}
void ShaderLibrary::Load(const std::string& name, std::filesystem::path vertexPath, std::filesystem::path fragmentPath) {
    SB_CORE_ASSERT(m_Shaders.contains(name), "Shader {0} already exists", name);
    Ref<Shader> shader = Shader::Create(std::move(vertexPath), std::move(fragmentPath));
    m_Shaders.emplace(name, std::move(shader));
}
void ShaderLibrary::Load(const std::string& name, std::filesystem::path path) {
    SB_CORE_ASSERT(m_Shaders.contains(name), "Shader {0} already exists", name);
    Ref<Shader> shader = Shader::Create(std::move(path));
    m_Shaders.emplace(name, std::move(shader));
}
void ShaderLibrary::Load(const std::string& name, std::string vertexSource, std::string fragmentSource) {
    SB_CORE_ASSERT(m_Shaders.contains(name), "Shader {0} already exists", name);
    Ref<Shader> shader = Shader::Create(std::move(vertexSource), std::move(fragmentSource), name);
    m_Shaders.emplace(name, std::move(shader));
}

Ref<Shader> ShaderLibrary::Get(const std::string& name) const {
    SB_CORE_ASSERT(m_Shaders.contains(name), "Shader {0} already exists", name);
    return m_Shaders.at(name);
}

}
