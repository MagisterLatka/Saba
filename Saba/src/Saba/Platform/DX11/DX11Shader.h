#pragma once

#include "Saba/Renderer/Shader.h"

#include <d3d11.h>

namespace Saba {

class DX11Shader : public Shader
{
public:
    SB_CORE DX11Shader(std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
    SB_CORE DX11Shader(std::filesystem::path path);
    SB_CORE DX11Shader(std::string vertexSource, std::string fragmentSource, std::string debugName);
    SB_CORE ~DX11Shader() = default;

    SB_CORE void Reload() override;
    SB_CORE void Reload(std::string vertexSource, std::string fragmentSource) override;

    SB_CORE void Bind() const noexcept override;

    SB_CORE const std::string& GetName() const noexcept override { return m_Name; };

    SB_CORE void AddReloadedCallback(ShaderReloadedCallback callback) noexcept override;

    SB_CORE ComPtr<ID3DBlob> GetVertexSource() const noexcept { return m_VSource; }
private:
    SB_CORE void PreProcess();
    SB_CORE void CompileFromSource();
private:
    std::vector<ShaderReloadedCallback> m_ShaderReloadedCallback;
    std::filesystem::path m_VPath, m_FPath;
    std::string m_Name, m_VsSource, m_FsSource;
    ComPtr<ID3D11VertexShader> m_VShader;
    ComPtr<ID3D11PixelShader> m_FShader;
    ComPtr<ID3DBlob> m_VSource;
    bool m_Loaded = false, m_UseFilepath = false;
};

}
