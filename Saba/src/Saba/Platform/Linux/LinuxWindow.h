#pragma once

#include "Saba/Core/Window.h"
#include "Saba/Platform/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>

namespace Saba {

class LinuxWindow : public Window {
    friend class OpenGLContext;
    friend class ImGuiLayer;
public:
    SB_CORE LinuxWindow(const WindowProps& props);
    LinuxWindow(const LinuxWindow&) = delete;
    LinuxWindow& operator=(const LinuxWindow&) = delete;
    SB_CORE ~LinuxWindow();

    SB_CORE void OnUpdate() override;
    SB_CORE void BindWindow() noexcept override;
    SB_CORE void BindToRender() noexcept override;
    SB_CORE void Clear(const glm::vec4& color) noexcept override;
    SB_CORE static std::optional<int> ProcessEvents();

    SB_CORE void SetTitle(const std::string& title) override;
    SB_CORE const std::string& GetTitle() const noexcept override { return m_Data.title; }

    SB_CORE void SetEventCallback(EventCallbackFn callback) noexcept override { m_Data.eventCallback = std::move(callback); }
    SB_CORE void SetTitleBarHitTestCallback(TitleBarHitTestCallbackFn callback) noexcept override { m_Data.titlebarHitTest = std::move(callback); }

    SB_CORE void EnableVSync() noexcept override;
    SB_CORE void DisableVSync() noexcept override;
    SB_CORE bool IsVSyncEnabled() const noexcept override { return m_Data.vSync; }

    SB_CORE void Minimize() noexcept override;
    SB_CORE bool IsMinimized() const noexcept override { return m_Data.minimized; }
    SB_CORE void Maximize() noexcept override;
    SB_CORE void Restore() noexcept override;
    SB_CORE bool IsMaximized() const noexcept override { return m_Data.maximized; }

    SB_CORE void SetIcon(const std::filesystem::path& iconPath) override;

    SB_CORE uint32_t GetWidth() const noexcept override { return m_Data.width; }
    SB_CORE uint32_t GetHeight() const noexcept override { return m_Data.height; }

    SB_CORE int GetXClientPos() const noexcept override { return m_Data.pos.x; }
    SB_CORE int GetYClientPos() const noexcept override { return m_Data.pos.y; }

    SB_CORE const Keyboard& GetKeyboard() const noexcept override { return m_Data.keyboard; }
    SB_CORE const Mouse& GetMouse() const noexcept override { return m_Data.mouse; }
private:
    SB_CORE void Init(const WindowProps& props);
    SB_CORE void Shutdown();

    SB_CORE void DefaultEventCallback([[maybe_unused]] Event& e) {}
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

}
