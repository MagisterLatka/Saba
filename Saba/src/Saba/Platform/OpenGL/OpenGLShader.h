#pragma once

#include "Saba/Renderer/Shader.h"

namespace Saba {

class OpenGLShader : public Shader {
public:
    OpenGLShader(std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
    OpenGLShader(std::filesystem::path path);
    OpenGLShader(std::string vertexSource, std::string fragmentSource, std::string debugName);
    ~OpenGLShader();

    void Reload() override;
    void Reload(std::string vertexSource, std::string fragmentSource) override;

    void Bind() const noexcept override;
    
    const std::string& GetName() const noexcept override { return m_Name; }

    void AddReloadedCallback(ShaderReloadedCallback callback) noexcept override;
private:
    void PreProcess();
    void Compile();
private:
    std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;
    std::filesystem::path m_VPath, m_FPath;
    std::string m_Name, m_VSource, m_FSource;
    uint32_t m_ID;
    bool m_Loaded = false, m_UseFilepath = false;
};

} //namespace Saba
