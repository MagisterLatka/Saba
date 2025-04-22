#include <utility>

#pragma once

namespace Saba {

class SabaException : public std::exception
{
public:
    SabaException(int line, const char* file, bool core = false) noexcept
        : m_File(file), m_Line(line), m_Core(core) {}
    const char* what() const noexcept override {
        std::ostringstream oss;
        oss << GetType() << std::endl;
        oss << GetOriginString();
        m_Buffer = oss.str();
        return m_Buffer.c_str();
    }
    virtual const char* GetType() const noexcept { return m_Core ? "CoreException" : "Exception"; }
    int GetLine() const noexcept { return m_Line; }
    const std::string& GetFile() const noexcept { return m_File; }
    std::string GetOriginString() const noexcept {
        std::ostringstream oss;
        oss << "[File] " << m_File << std::endl;
        oss << "[Line] " << m_Line;
        return oss.str();
    }
protected:
    mutable std::string m_Buffer;
private:
    std::string m_File;
    int m_Line;
    bool m_Core;
};
class MessageException : public SabaException
{
public:
    MessageException(int line, const char* file, std::string  message, bool core = false) noexcept
        : SabaException(line, file, core), m_Message(std::move(message)) {}
    const char* what() const noexcept override {
        std::ostringstream oss;
        oss << GetType() << std::endl;
        oss << "[Message] " << m_Message << std::endl;
        oss << GetOriginString();
        m_Buffer = oss.str();
        return m_Buffer.c_str();
    }
private:
    std::string m_Message;
};

} //namespace Saba
