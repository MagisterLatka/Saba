#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Core/Window.h"
#include "Saba/Renderer/GraphicsContext.h"

#include <d3d11.h>

namespace Saba {

class WindowsWindow : public Window {
    friend class OpenGLContext;
    friend class DX11Context;
    friend class ImGuiLayer;
public:
    class WindowException : public SabaException {
    public:
        SB_CORE WindowException(int line, const char* file, HRESULT hresult) noexcept;
        SB_CORE const char* what() const noexcept override;
        SB_CORE const char* GetType() const noexcept override { return "Window Exception"; }
        SB_CORE HRESULT GetErrorCode() const noexcept { return m_Result; }
        SB_CORE std::string GetErrorDescription() const noexcept;
        SB_CORE static std::string TranslateErrorCode(HRESULT hr) noexcept;
    private:
        HRESULT m_Result;
    };
    class WindowClass {
    public:
        SB_CORE static const char* GetName() noexcept { return "Saba Engine Window Class"; }
        SB_CORE static HINSTANCE GetInstance() noexcept { return s_WindowClass.m_Instance; }
    private:
        SB_CORE WindowClass() noexcept;
        SB_CORE ~WindowClass();
    private:
        SB_CORE static WindowClass s_WindowClass;
        HINSTANCE m_Instance;
    };
public:
    SB_CORE WindowsWindow(const WindowProps& props);
    WindowsWindow(const WindowsWindow&) = delete;
    WindowsWindow& operator=(const WindowsWindow&) = delete;
    SB_CORE ~WindowsWindow();

    SB_CORE void OnUpdate() override;
    SB_CORE void BindWindow() noexcept override {}
    SB_CORE void BindToRender() noexcept override;
    SB_CORE void Clear(const glm::vec4& color) noexcept override;
    SB_CORE static std::optional<int> ProcessEvents();

    SB_CORE void SetTitle(const std::string& title) override;
    SB_CORE const std::string& GetTitle() const noexcept override { return m_Data.title; }

    SB_CORE void SetEventCallback(EventCallbackFn callback) noexcept override { m_Data.eventCallback = std::move(callback); }
    SB_CORE void SetTitleBarHitTestCallback(TitleBarHitTestCallbackFn callback) noexcept override { m_Data.titleBarHitTest = std::move(callback); }

    SB_CORE void EnableVSync() noexcept override { m_Data.vSync = true; }
    SB_CORE void DisableVSync() noexcept override { m_Data.vSync = false; }
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

    SB_CORE const Keyboard& GetKeyboard() const noexcept override { return m_Keyboard; }
    SB_CORE const Mouse& GetMouse() const noexcept override { return m_Mouse; }
private:
    SB_CORE void Init(const WindowProps& props);
    SB_CORE void Shutdown();

    SB_CORE std::optional<LRESULT> HitTest(BOOL hasThickFrame, LPARAM lParam);

    SB_CORE static HICON CreateIcon(const uint8_t* pixels, int x, int y);

    SB_CORE static LRESULT CALLBACK HandleMsgSetup(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    SB_CORE static LRESULT CALLBACK HandleMsgCall(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
    SB_CORE LRESULT HandleMsg(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
    SB_CORE void HandleImGuiInput(bool handle = true) noexcept { m_HandleImGuiInput = handle; }

    SB_CORE void DefaultEventCallback([[maybe_unused]] Event& e) {}
private:
    struct WindowData {
        std::string title;
        uint32_t width, height;
        glm::ivec2 pos;
        bool vSync = false, maximized = false, minimized = false, titlebar = true;

        EventCallbackFn eventCallback;
        TitleBarHitTestCallbackFn titleBarHitTest;
    } m_Data;

    Keyboard m_Keyboard;
    Mouse m_Mouse;

    HWND m_Window = nullptr;
    HDC m_DC = nullptr;
    HGLRC m_Context = nullptr;
    HICON m_SmallIcon = nullptr, m_BigIcon = nullptr;

    bool m_HandleImGuiInput = false;
};

}

#define SB_WINDOWS_WINDOW_EXCEPTION(hr) Saba::WindowsWindow::WindowException(__LINE__, __FILE__, hr)
#define SB_WINDOWS_WINDOW_LAST_EXCEPTION() Saba::WindowsWindow::WindowException(__LINE__, __FILE__, GetLastError())
