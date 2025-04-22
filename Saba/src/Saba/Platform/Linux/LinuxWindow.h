#pragma once

#include "Saba/Core/Window.h"
#include "Saba/Platform/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>

namespace Saba {

class LinuxWindow : public Window {
    friend class OpenGLContext;
    friend class ImGuiLayer;
public:
    LinuxWindow(const WindowProps& props);
    LinuxWindow(const LinuxWindow&) = delete;
    LinuxWindow& operator=(const LinuxWindow&) = delete;
    ~LinuxWindow();

    void OnUpdate() override;
    void BindWindow() noexcept override;
    void BindToRender() noexcept override;
    void Clear(const glm::vec4& color) noexcept override;
    static std::optional<int> ProcessEvents();

    void SetTitle(const std::string& title) override;
    const std::string& GetTitle() const noexcept override { return m_Data.title; }

    void SetEventCallback(EventCallbackFn callback) noexcept override { m_Data.eventCallback = std::move(callback); }
    void SetTitleBarHitTestCallback(TitleBarHitTestCallbackFn callback) noexcept override { m_Data.titlebarHitTest = std::move(callback); }

    void EnableVSync() noexcept override;
    void DisableVSync() noexcept override;
    bool IsVSyncEnabled() const noexcept override { return m_Data.vSync; }

    void Minimize() noexcept override;
    bool IsMinimized() const noexcept override { return m_Data.minimized; }
    void Maximize() noexcept override;
    void Restore() noexcept override;
    bool IsMaximized() const noexcept override { return m_Data.maximized; }

    void SetIcon(const std::filesystem::path& iconPath) override;

    uint32_t GetWidth() const noexcept override { return m_Data.width; }
    uint32_t GetHeight() const noexcept override { return m_Data.height; }

    int GetXClientPos() const noexcept override { return m_Data.pos.x; }
    int GetYClientPos() const noexcept override { return m_Data.pos.y; }

    const Keyboard& GetKeyboard() const noexcept override { return m_Data.keyboard; }
    const Mouse& GetMouse() const noexcept override { return m_Data.mouse; }
private:
    void Init(const WindowProps& props);
    void Shutdown();

    void DefaultEventCallback([[maybe_unused]] Event& e) {}
private:
    struct WindowData {
        std::string title;
        EventCallbackFn eventCallback;
        TitleBarHitTestCallbackFn titlebarHitTest;
        uint32_t width, height;
        glm::ivec2 pos;
        bool vSync = false, maximized = false, minimized = false, titlebar = true;

        Keyboard keyboard;
        Mouse mouse;
    } m_Data;

    GLFWwindow* m_Window = nullptr;
};

} //namespace Saba
