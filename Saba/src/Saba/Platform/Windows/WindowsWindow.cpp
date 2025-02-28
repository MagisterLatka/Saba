#include <pch.h>
#include "WindowsWindow.h"

#include "Saba/Events/ApplicationEvents.h"
#include "Saba/Events/KeyEvents.h"
#include "Saba/Events/MouseEvents.h"
#include "Saba/Core/Application.h"

#include <imgui.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Saba {

WindowsWindow::WindowsWindow(const WindowProps& props) {
    Init(props);
}
WindowsWindow::~WindowsWindow() {
    Shutdown();
}

void WindowsWindow::OnUpdate() {
    Application::Get().GetGraphicsContext()->SwapBuffers(this);
}
std::optional<int> WindowsWindow::ProcessEvents() {
    MSG msg;
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT)
            return static_cast<int>(msg.wParam);

        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return {};
}

void WindowsWindow::BindToRender() noexcept {
    Application::Get().GetGraphicsContext()->BindToRender(this);
}
void WindowsWindow::Clear(const glm::vec4& color) noexcept {
    Application::Get().GetGraphicsContext()->Clear(this, color);
}

void WindowsWindow::SetTitle(const std::string& title) {
    if (SetWindowTextA(m_Window, title.c_str()) == 0)
        throw SB_WINDOWS_WINDOW_LAST_EXCEPTION();
    m_Data.title = title;
}

void WindowsWindow::Init(const WindowProps& props) {
    m_Data.title = props.Title;
    m_Data.width = props.Width;
    m_Data.height = props.Height;
    m_Data.eventCallback = SB_BIND_EVENT_FN(WindowsWindow::DefaultEventCallback);

    SB_CORE_INFO("Creating window {0}, ({1}, {2})", m_Data.title, m_Data.width, m_Data.height);

    RECT windowArea;
    windowArea.left = 0;
    windowArea.right = m_Data.width;
    windowArea.top = 0;
    windowArea.bottom = m_Data.height;
    if (AdjustWindowRect(&windowArea, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU, false) == 0)
        throw SB_WINDOWS_WINDOW_LAST_EXCEPTION();

    m_Window = CreateWindowExA(0u, WindowClass::GetName(), m_Data.title.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, windowArea.right - windowArea.left, windowArea.bottom - windowArea.top, nullptr, nullptr,
        WindowClass::GetInstance(), this);
    if (m_Window == nullptr)
        throw SB_WINDOWS_WINDOW_LAST_EXCEPTION();

    m_DC = GetDC(m_Window);

    Application::Get().GetGraphicsContext()->InitForWindow(this);

    ShowWindow(m_Window, SW_SHOW);
}
void WindowsWindow::Shutdown() {
    Application::Get().GetGraphicsContext()->ShutdownForWindow(this);
    DestroyWindow(m_Window);
}

LRESULT WindowsWindow::HandleMsgSetup(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    if (msg == WM_CREATE) {
        const CREATESTRUCTA* create = reinterpret_cast<CREATESTRUCTA*>(lParam);
        WindowsWindow* window = reinterpret_cast<WindowsWindow*>(create->lpCreateParams);
        SetWindowLongPtrA(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        SetWindowLongPtrA(windowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsWindow::HandleMsgCall));
        return window->HandleMsg(windowHandle, msg, wParam, lParam);
    }
    return DefWindowProcA(windowHandle, msg, wParam, lParam);
}
LRESULT WindowsWindow::HandleMsgCall(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowsWindow* window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtrA(windowHandle, GWLP_USERDATA));
    return window->HandleMsg(windowHandle, msg, wParam, lParam);
}
LRESULT WindowsWindow::HandleMsg(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (m_HandleImGuiInput) {
        if (ImGui_ImplWin32_WndProcHandler(windowHandle, msg, wParam, lParam))
            return true;
    }

    switch (msg)
    {
        case WM_CLOSE: {
            WindowCloseEvent e;
            m_Data.eventCallback(e);
            PostQuitMessage(0);
            return 0;
        }
        case WM_KILLFOCUS: {
            m_Keyboard.ClearState();
            WindowLostFocusEvent e;
            m_Data.eventCallback(e);
            break;
        }
        case WM_SETFOCUS: {
            WindowFocusEvent e;
            m_Data.eventCallback(e);
            break;
        }
        case WM_SIZE: {
            uint32_t width = LOWORD(lParam), height = HIWORD(lParam);
            m_Data.width = width;
            m_Data.height = height;
            WindowResizeEvent e(width, height);
            m_Data.eventCallback(e);
            break;
        }
        case WM_MOVE: {
            int xpos = LOWORD(lParam), ypos = HIWORD(lParam);
            m_Data.pos = { xpos, ypos };
            WindowMovedEvent e(xpos, ypos);
            m_Data.eventCallback(e);
            break;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: {
            if (!(lParam & 0x40000000) || m_Keyboard.IsAutorepeatEnabled())
                m_Keyboard.OnKeyPressed(static_cast<uint8_t>(wParam));
            KeyPressedEvent e(KeyCode(static_cast<uint8_t>(wParam)), LOWORD(lParam));
            m_Data.eventCallback(e);
            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            m_Keyboard.OnKeyReleased(static_cast<uint8_t>(wParam));
            KeyReleasedEvent e(KeyCode(static_cast<uint8_t>(wParam)));
            m_Data.eventCallback(e);
            break;
        }
        case WM_CHAR: {
            m_Keyboard.OnChar(static_cast<uint8_t>(wParam));
            KeyTypedEvent e(static_cast<uint8_t>(wParam));
            m_Data.eventCallback(e);
            break;
        }

        case WM_MOUSEMOVE: {
            const POINTS points = MAKEPOINTS(lParam);
            MouseMovedEvent e(points.x, points.y);
            if (points.x >= 0 && points.x < (uint16_t)m_Data.width &&
                points.y >= 0 && points.y < (uint16_t)m_Data.height)
            {
                if (!m_Mouse.IsInWindow()) {
                    SetCapture(m_Window);
                    m_Mouse.OnMouseEnter();
                    MouseEnteredEvent ev;
                    m_Data.eventCallback(ev);
                }
                m_Mouse.OnMouseMove(points.x, points.y);
                m_Data.eventCallback(e);
            }
            else if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
                m_Mouse.OnMouseMove(points.x, points.y);
                m_Data.eventCallback(e);
            }
            else {
                ReleaseCapture();
                m_Mouse.OnMouseLeave();
                MouseLeftEvent ev;
                m_Data.eventCallback(ev);
            }
            break;
        }
        case WM_LBUTTONDOWN: {
            m_Mouse.OnLeftPressed();
            MouseButtonPressedEvent e(MouseCode::ButtonLeft);
            m_Data.eventCallback(e);
            break;
        }
        case WM_LBUTTONUP: {
            const POINTS points = MAKEPOINTS(lParam);
            m_Mouse.OnLeftReleased();
            if (points.x < 0 && points.x >= (uint16_t)m_Data.width &&
                points.y < 0 && points.y >= (uint16_t)m_Data.height)
            {
                ReleaseCapture();
                m_Mouse.OnMouseLeave();
                MouseLeftEvent ev;
                m_Data.eventCallback(ev);
            }
            MouseButtonReleasedEvent e(MouseCode::ButtonLeft);
            m_Data.eventCallback(e);
            break;
        }
        case WM_RBUTTONDOWN: {
            m_Mouse.OnRightPressed();
            MouseButtonPressedEvent e(MouseCode::ButtonRight);
            m_Data.eventCallback(e);
            break;
        }
        case WM_RBUTTONUP: {
            const POINTS points = MAKEPOINTS(lParam);
            m_Mouse.OnRightReleased();
            if (points.x < 0 && points.x >= (uint16_t)m_Data.width &&
                points.y < 0 && points.y >= (uint16_t)m_Data.height)
            {
                ReleaseCapture();
                m_Mouse.OnMouseLeave();
                MouseLeftEvent ev;
                m_Data.eventCallback(ev);
            }
            MouseButtonReleasedEvent e(MouseCode::ButtonRight);
            m_Data.eventCallback(e);
            break;
        }
        case WM_MOUSEWHEEL: {
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            m_Mouse.OnWheelDelta(delta);
            MouseScrolledEvent e(0, static_cast<float>(delta));
            m_Data.eventCallback(e);
            break;
        }
    }
    return DefWindowProcA(windowHandle, msg, wParam, lParam);
}

WindowsWindow::WindowClass WindowsWindow::WindowClass::s_WindowClass;
WindowsWindow::WindowClass::WindowClass() noexcept
    : m_Instance(GetModuleHandle(nullptr))
{
    WNDCLASSEXA windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEXA);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowsWindow::HandleMsgSetup;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = m_Instance;
    windowClass.hIcon = nullptr;
    windowClass.hCursor = nullptr;
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = GetName();
    windowClass.hIconSm = nullptr;

    RegisterClassExA(&windowClass);
}
WindowsWindow::WindowClass::~WindowClass() {
    UnregisterClassA(GetName(), m_Instance);
}

WindowsWindow::WindowException::WindowException(int line, const char* file, HRESULT hresult) noexcept
    : SabaException(line, file, true), m_Result(hresult) {}

const char* WindowsWindow::WindowException::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl;
    oss << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec << " (" << (uint32_t)GetErrorCode() << ")" << std::endl;
    oss << "[Description] " << GetErrorDescription() << std::endl;
    oss << GetOriginString();
    m_Buffer = oss.str();
    return m_Buffer.c_str();
}

std::string WindowsWindow::WindowException::GetErrorDescription() const noexcept {
    return TranslateErrorCode(m_Result);
}
std::string WindowsWindow::WindowException::TranslateErrorCode(HRESULT hr) noexcept {
    char* msgBuffer = nullptr;
    const uint32_t msgLength = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&msgBuffer), 0, nullptr);
    
    if (msgLength == 0)
        return "Unidentified error code";
    
    std::string errorString = msgBuffer;
    LocalFree(msgBuffer);
    return errorString;
}

}
