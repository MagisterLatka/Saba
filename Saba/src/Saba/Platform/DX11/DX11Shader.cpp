#include <pch.h>
#include "DX11Shader.h"

#include "Saba/Platform/DX11/DX11Context.h"
#include "Saba/Renderer/Renderer.h"
#include "Saba/Utils/FileProcessing.h"

#include <d3dcompiler.h>

namespace Saba {

static std::string GetNameFromFilepath(const std::filesystem::path& filepath) {
    std::string path = filepath.string();
    std::string name;
    uint64_t found = path.find_last_of("/\\");
    name = found != std::string::npos ? path.substr(found + 1) : path;
    found = name.rfind(".vert.cso");
    if (found == std::string::npos)
        found = name.rfind(".hshader");
    name = found != std::string::npos ? name.substr(0, found) : name;
    return name;
}
DX11Shader::DX11Shader(std::filesystem::path vertexPath, std::filesystem::path fragmentPath)
    : m_VPath(std::move(vertexPath)), m_FPath(std::move(fragmentPath)), m_Name(GetNameFromFilepath(vertexPath)), m_UseFilepath(true)
{
    Reload();
}
DX11Shader::DX11Shader(std::filesystem::path path)
    : m_VPath(std::move(path)), m_Name(GetNameFromFilepath(path)), m_UseFilepath(true)
{
    Reload();
}
DX11Shader::DX11Shader(std::string vertexSource, std::string fragmentSource, std::string debugName)
    : m_Name(std::move(debugName))
{
    Reload(std::move(vertexSource), std::move(fragmentSource));
}
void DX11Shader::Reload() {
    Ref<DX11Shader> instance = this;
    Renderer::Submit([instance]() mutable {
        HRESULT hr;
        auto device = DX11Context::GetContextFromApplication()->GetDevice();

        if (instance->m_UseFilepath) {
            if (instance->m_FPath.empty()) {
                instance->PreProcess();
                instance->CompileFromSource();
            }
            else {
                SB_DX_GRAPHICS_CALL_INFO(D3DReadFileToBlob(instance->m_FPath.c_str(), &instance->m_VSource));
                SB_DX_GRAPHICS_CALL_INFO(device->CreatePixelShader(instance->m_VSource->GetBufferPointer(), instance->m_VSource->GetBufferSize(), nullptr, &instance->m_FShader));
                SB_DX_GRAPHICS_CALL_INFO(D3DReadFileToBlob(instance->m_VPath.c_str(), &instance->m_VSource));
                SB_DX_GRAPHICS_CALL_INFO(device->CreateVertexShader(instance->m_VSource->GetBufferPointer(), instance->m_VSource->GetBufferSize(), nullptr, &instance->m_VShader));
            }
        }
        else return;

        if (instance->m_Loaded) {
            for (auto& callback : instance->m_ShaderReloadedCallback)
                callback();
        }
        instance->m_Loaded = true;
    });
}
void DX11Shader::Reload(std::string vertexSource, std::string fragmentSource)
{
    m_VsSource = std::move(vertexSource);
    m_FsSource = std::move(fragmentSource);

    Ref<DX11Shader> instance = this;
    Renderer::Submit([instance]() mutable {
        instance->CompileFromSource();

        if (instance->m_Loaded) {
            for (auto& callback : instance->m_ShaderReloadedCallback)
                callback();
        }
        instance->m_Loaded = true;
    });
}
void DX11Shader::PreProcess()
{
    std::string source = FileProcessing::ReadFromFile(m_VPath);

    static const std::string typeToken = "#type";
    auto pos = source.find(typeToken);
    while (pos != std::string::npos) {
        const auto eol = source.find_first_of("\r\n", pos);
        SB_CORE_ASSERT(eol != std::string::npos, "Shader syntax error");
        const auto begin = pos + typeToken.length() + 1;
        const std::string type = source.substr(begin, eol - begin);
        SB_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type");

        const auto nextLinePos = source.find_first_not_of("\r\n", eol);
        SB_CORE_ASSERT(nextLinePos != std::string::npos, "Shader syntax error");
        pos = source.find(typeToken, nextLinePos);
        std::string text = source.substr(nextLinePos, pos == std::string::npos ? std::string::npos : pos - nextLinePos);
        if (type == "vertex")
            m_VsSource = std::move(text);
        else
            m_FsSource = std::move(text);
    }
    SB_CORE_ASSERT(!m_VsSource.empty() && !m_FsSource.empty(), "Missing source for shader");
}
void DX11Shader::CompileFromSource()
{
    HRESULT hr;
    auto device = DX11Context::GetContextFromApplication()->GetDevice();

    uint32_t compileFlags = 0u;
#if defined(SB_DEBUG)
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    ID3DBlob* errorMsgs = nullptr;
    D3DCompile(m_FsSource.c_str(), m_FsSource.length(), nullptr, nullptr, nullptr, "main", "ps_5_0", compileFlags, 0u, &m_VSource, &errorMsgs);
    if (errorMsgs != nullptr) {
        std::string errors = "Vertex shader compilation failed: ";
        errors.append(reinterpret_cast<const char*>(errorMsgs->GetBufferPointer()), errorMsgs->GetBufferSize());
        SB_CORE_THROW_INFO(errors);
        errorMsgs->Release();
    }
    SB_DX_GRAPHICS_CALL_INFO(device->CreatePixelShader(m_VSource->GetBufferPointer(), m_VSource->GetBufferSize(), nullptr, &m_FShader));

    D3DCompile(m_VsSource.c_str(), m_VsSource.length(), nullptr, nullptr, nullptr, "main", "vs_5_0", compileFlags, 0u, &m_VSource, &errorMsgs);
    if (errorMsgs != nullptr) {
        std::string errors = "Fragment shader compilation failed: ";
        errors.append(reinterpret_cast<const char*>(errorMsgs->GetBufferPointer()), errorMsgs->GetBufferSize());
        SB_CORE_THROW_INFO(errors);
        errorMsgs->Release();
    }
    SB_DX_GRAPHICS_CALL_INFO(device->CreateVertexShader(m_VSource->GetBufferPointer(), m_VSource->GetBufferSize(), nullptr, &m_VShader));

    m_VsSource.clear();
    m_FsSource.clear();
}

void DX11Shader::Bind() const noexcept {
    Ref<const DX11Shader> instance = this;
    Renderer::Submit([instance]() {
        auto context = DX11Context::GetContextFromApplication()->GetContext();
        context->VSSetShader(instance->m_VShader.Get(), nullptr, 0u);
        context->PSSetShader(instance->m_FShader.Get(), nullptr, 0u);
    });
}

void DX11Shader::AddReloadedCallback(ShaderReloadedCallback callback) noexcept {
    m_ShaderReloadedCallback.push_back(std::move(callback));
}

}
