#include <pch.h>
#include "OpenGLShader.h"

#include "Saba/Renderer/Renderer.h"
#include "Saba/Utils/FileProcessing.h"

#include <glad/glad.h>

namespace Saba {

static std::string GetNameFromFilepath(const std::filesystem::path& filepath) {
    std::string path = filepath.string();
    if (auto found = path.find_last_of("/\\"))
        path.erase(0, found);
    if (auto found = path.rfind('.'))
        path.erase(found + 1);
    return path;
}

OpenGLShader::OpenGLShader(std::filesystem::path vertexPath, std::filesystem::path fragmentPath)
    : m_VPath(std::move(vertexPath)), m_FPath(std::move(fragmentPath)), m_Name(GetNameFromFilepath(m_VPath)), m_UseFilepath(true)
{
    Reload();
}
OpenGLShader::OpenGLShader(std::filesystem::path path)
    : m_VPath(std::move(path)), m_Name(GetNameFromFilepath(m_VPath)), m_UseFilepath(true)
{
    Reload();
}
OpenGLShader::OpenGLShader(std::string vertexSource, std::string fragmentSource, std::string debugName)
    : m_Name(std::move(debugName))
{
    Reload(std::move(vertexSource), std::move(fragmentSource));
}
OpenGLShader::~OpenGLShader() {
    glDeleteProgram(m_ID);
}

void OpenGLShader::Reload() {
    if (m_UseFilepath) {
        if (m_FPath.empty()) {
            PreProcess();
        }
        else {
            m_VSource = FileProcessing::ReadFromFile(m_VPath);
            m_FSource = FileProcessing::ReadFromFile(m_FPath);
        }
    }
    else return;

    Compile();
}
void OpenGLShader::Reload(std::string vertexSource, std::string fragmentSource) {
    m_VSource = std::move(vertexSource);
    m_FSource = std::move(fragmentSource);

    Compile();
}

void OpenGLShader::PreProcess() {
    std::string source = FileProcessing::ReadFromFile(m_VPath);

    static const std::string typeToken("#type"); //not constexpr bcs msvc is dumb af :c
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
            m_VSource = std::move(text);
        else
            m_FSource = std::move(text);
    }
    SB_CORE_ASSERT(!m_VSource.empty() && !m_FSource.empty(), "Missing source for shader");
}
void OpenGLShader::Compile() {
    Ref<OpenGLShader> instance = this;
    Renderer::Submit([instance]() mutable {
        instance->m_ID = glCreateProgram();

        uint32_t vertexID = glCreateShader(GL_VERTEX_SHADER);
        const char* vSource = instance->m_VSource.c_str();
        glShaderSource(vertexID, 1, &vSource, nullptr);
        glCompileShader(vertexID);
        int isCompiled = 0;
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
            glGetShaderInfoLog(vertexID, maxLength, &maxLength, infoLog.data());

            glDeleteShader(vertexID);
            SB_CORE_THROW_INFO(std::string("Shader compilation failed (" + instance->m_VPath.string() + "):\n" + infoLog.data()));
        }
        glAttachShader(instance->m_ID, vertexID);

        uint32_t fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fSource = instance->m_FSource.c_str();
        glShaderSource(fragmentID, 1, &fSource, nullptr);
        glCompileShader(fragmentID);
        isCompiled = 0;
        glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
            glGetShaderInfoLog(fragmentID, maxLength, &maxLength, infoLog.data());

            glDeleteShader(fragmentID);
            SB_CORE_THROW_INFO(std::string("Shader compilation failed (" +
                (instance->m_FPath.empty() ? instance->m_VPath.string() : instance->m_FPath.string()) +
                "):\n" + infoLog.data()));
        }
        glAttachShader(instance->m_ID, fragmentID);

        glLinkProgram(instance->m_ID);
        glValidateProgram(instance->m_ID);

        glDetachShader(instance->m_ID, vertexID);
        glDetachShader(instance->m_ID, fragmentID);
        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);

        int isLinked = 0;
        glGetProgramiv(instance->m_ID, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(instance->m_ID, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
            glGetProgramInfoLog(instance->m_ID, maxLength, &maxLength, infoLog.data());
            SB_CORE_ERROR("Shader linking failed ({0}):\n{1}", instance->m_VPath.string(), infoLog.data());
            glDeleteProgram(instance->m_ID);
        }

        if (instance->m_Loaded) {
            for (auto& callback : instance->m_ShaderReloadedCallbacks)
                callback();
        }
        instance->m_Loaded = true;
    });
}

void OpenGLShader::Bind() const noexcept {
    Ref<const OpenGLShader> instance = this;
    Renderer::Submit([instance]() {
        glUseProgram(instance->m_ID);
    });
}

void OpenGLShader::AddReloadedCallback(ShaderReloadedCallback callback) noexcept {
    m_ShaderReloadedCallbacks.push_back(std::move(callback));
}

}
