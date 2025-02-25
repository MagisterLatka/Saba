#pragma once

#include "Saba/Renderer/Shader.h"

namespace Saba {

class OpenGLShader : public Shader {
public:
    SB_CORE OpenGLShader(std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
    SB_CORE OpenGLShader(std::filesystem::path path);
    SB_CORE OpenGLShader(std::string vertexSource, std::string fragmentSource, std::string debugName);
    SB_CORE ~OpenGLShader();

    SB_CORE void Reload() override;
    SB_CORE void Reload(std::string vertexSource, std::string fragmentSource) override;

    SB_CORE void Bind() const noexcept override;
    
    SB_CORE const std::string& GetName() const noexcept override { return m_Name; }

    SB_CORE void AddReloadedCallback(ShaderReloadedCallback callback) noexcept override;
private:
    SB_CORE void PreProcess();
    SB_CORE void Compile();
private:
    std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;
    std::filesystem::path m_VPath, m_FPath;
    std::string m_Name, m_VSource, m_FSource;
    uint32_t m_ID;
    bool m_Loaded = false, m_UseFilepath = false;
};

}
