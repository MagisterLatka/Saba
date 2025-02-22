#pragma once

#include "Saba/Core/Window.h"
#include "Saba/Platform/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>

namespace Saba {

class LinuxWindow : public Window {
public:
    SB_CORE LinuxWindow(const WindowProps& props);
    LinuxWindow(const LinuxWindow&) = delete;
    LinuxWindow& operator=(const LinuxWindow&) = delete;
    SB_CORE ~LinuxWindow();

    SB_CORE void OnUpdate() override;
    SB_CORE void BindToRender() noexcept override;
    SB_CORE void Clear(const glm::vec4& color) noexcept override;
    SB_CORE static std::optional<int> ProcessEvents();

    SB_CORE void SetTitle(const std::string& title) override;
    SB_CORE const std::string& GetTitle() const noexcept override { return m_Data.title; }

    SB_CORE void SetEventCallback(EventCallbackFn callback) noexcept override { m_Data.eventCallback = std::move(callback); }
    SB_CORE void EnableVSync() noexcept override;
    SB_CORE void DisableVSync() noexcept override;
    SB_CORE bool IsVSyncEnabled() const noexcept override { return m_Data.vSync; }

    SB_CORE uint32_t GetWidth() const noexcept override { return m_Data.width; }
    SB_CORE uint32_t GetHeight() const noexcept override { return m_Data.height; }

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
        uint32_t width, height;
        glm::ivec2 pos;
        bool vSync = false;

        Keyboard keyboard;
        Mouse mouse;
    } m_Data;

    GLFWwindow* m_Window = nullptr;
    Ref<OpenGLContext> m_Context;
};

}
