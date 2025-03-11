#include <pch.h>
#include "WindowsWindow.h"

#include "Saba/Events/ApplicationEvents.h"
#include "Saba/Events/KeyEvents.h"
#include "Saba/Events/MouseEvents.h"
#include "Saba/Core/Application.h"

#include <imgui.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include <stb_image.h>

#include <shellapi.h>

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

void WindowsWindow::Minimize() noexcept {
    m_Data.minimized = true;
    ShowWindow(m_Window, SW_MINIMIZE);
}
void WindowsWindow::Maximize() noexcept {
    m_Data.maximized = true;
    ShowWindow(m_Window, SW_MAXIMIZE);
}
void WindowsWindow::Restore() noexcept {
    m_Data.maximized = false;
    ShowWindow(m_Window, SW_RESTORE);
}

void WindowsWindow::SetIcon(const std::filesystem::path& iconPath) {
    std::string path = iconPath.string();
    if (std::filesystem::exists(iconPath)) {
        int x, y, channels;
        uint8_t* pixels = stbi_load(path.c_str(), &x, &y, &channels, 4);

        HICON bigIcon, smallIcon;
        bigIcon = CreateIcon(pixels, x, y);
        smallIcon = CreateIcon(pixels, x, y);

        SendMessageA(m_Window, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(bigIcon));
        SendMessageA(m_Window, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(smallIcon));

        if (m_SmallIcon) DestroyIcon(m_SmallIcon);
        m_SmallIcon = smallIcon;
        if (m_BigIcon) DestroyIcon(m_BigIcon);
        m_BigIcon = bigIcon;

        stbi_image_free(pixels);
    }
    else SB_CORE_WARN("Path {0} does not exist", path);
}
HICON WindowsWindow::CreateIcon(const uint8_t* pixels, int x, int y) {
    BITMAPV5HEADER bi = { 0 };
    bi.bV5Size = sizeof(bi);
    bi.bV5Width = x;
    bi.bV5Height = -y;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask = 0x00ff0000;
    bi.bV5GreenMask = 0x0000ff00;
    bi.bV5BlueMask = 0x000000ff;
    bi.bV5AlphaMask = 0xff000000;

    HDC deviceContext = GetDC(nullptr);
    uint8_t* target = nullptr;
    HBITMAP color = CreateDIBSection(deviceContext, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS,
        reinterpret_cast<void**>(&target), nullptr, static_cast<DWORD>(0));
    ReleaseDC(nullptr, deviceContext);
    if (!color)
        throw SB_WINDOWS_WINDOW_LAST_EXCEPTION();

    HBITMAP mask = CreateBitmap(x, y, 1, 1, nullptr);
    if (!mask) {
        DeleteObject(color);
        SB_CORE_THROW("Failed to create mask bitmap");
    }

    uint8_t* source = (uint8_t*)pixels;
    for (int i = 0; i < x * y; ++i) {
        target[0] = source[2];
        target[1] = source[1];
        target[2] = source[0];
        target[3] = source[3];
        target += 4;
        source += 4;
    }

    ICONINFO iconInfo = { 0 };
    iconInfo.fIcon = TRUE;
    iconInfo.hbmColor = color;
    iconInfo.hbmMask = mask;

    HICON handle = CreateIconIndirect(&iconInfo);
    DeleteObject(color);
    DeleteObject(mask);
    SB_CORE_ASSERT(handle, "Failed to create icon");
    return handle;
}

void WindowsWindow::Init(const WindowProps& props) {
    m_Data.title = props.Title;
    m_Data.width = props.Width;
    m_Data.height = props.Height;
    m_Data.titlebar = props.HasTitleBar;
    m_Data.eventCallback = SB_BIND_EVENT_FN(WindowsWindow::DefaultEventCallback);

    SB_CORE_INFO("Creating window {0}, ({1}, {2})", m_Data.title, m_Data.width, m_Data.height);

    ////////////////////////////////////////////////////////////////////////////////////////////////

    DWORD windowStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION;
    if (props.Resizable) windowStyle |= WS_MAXIMIZEBOX | WS_THICKFRAME;
    if (props.Maximized) {
        windowStyle |= WS_MAXIMIZE;
        m_Data.maximized = true;
    }
    DWORD windowExStyle = WS_EX_ACCEPTFILES | WS_EX_APPWINDOW;

    RECT windowArea;
    windowArea.left = 0;
    windowArea.right = m_Data.width;
    windowArea.top = 0;
    windowArea.bottom = m_Data.height;
    if (AdjustWindowRectEx(&windowArea, windowStyle, false, windowExStyle) == 0)
        throw SB_WINDOWS_WINDOW_LAST_EXCEPTION();

    m_Window = CreateWindowExA(windowExStyle, WindowClass::GetName(), m_Data.title.c_str(), windowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
        windowArea.right - windowArea.left, windowArea.bottom - windowArea.top, nullptr, nullptr,
        WindowClass::GetInstance(), this);
    if (m_Window == nullptr)
        throw SB_WINDOWS_WINDOW_LAST_EXCEPTION();

    m_DC = GetDC(m_Window);

    ////////////////////////////////////////////////////////////////////////////////////////////////

    ChangeWindowMessageFilterEx(m_Window, WM_DROPFILES, MSGFLT_ALLOW, nullptr);
    ChangeWindowMessageFilterEx(m_Window, WM_COPYDATA, MSGFLT_ALLOW, nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////

    windowArea.left = 0;
    windowArea.right = m_Data.width + windowArea.left;
    windowArea.top = 0;
    windowArea.bottom = m_Data.height + windowArea.top;

    ClientToScreen(m_Window, reinterpret_cast<POINT*>(&windowArea.left));
    ClientToScreen(m_Window, reinterpret_cast<POINT*>(&windowArea.right));

    AdjustWindowRectEx(&windowArea, windowStyle, FALSE, windowExStyle);

    WINDOWPLACEMENT windowPlacement;
    windowPlacement.length = sizeof(WINDOWPLACEMENT);

    GetWindowPlacement(m_Window, &windowPlacement);
    windowPlacement.rcNormalPosition = windowArea;
    windowPlacement.showCmd = SW_HIDE;
    SetWindowPlacement(m_Window, &windowPlacement);

    DragAcceptFiles(m_Window, TRUE);

    GetClientRect(m_Window, &windowArea);
    m_Data.width = windowArea.right;
    m_Data.height = windowArea.bottom;

    ////////////////////////////////////////////////////////////////////////////////////////////////


    Application::Get().GetGraphicsContext()->InitForWindow(this);

    ShowWindow(m_Window, SW_SHOWNA);
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

    BOOL hasThickFrame = GetWindowLongPtrA(m_Window, GWL_STYLE) & WS_THICKFRAME;

    switch (msg)
    {
        case WM_CREATE: {
            if (!m_Data.titlebar && hasThickFrame) {
                RECT rect;
                GetWindowRect(m_Window, &rect);
                SetWindowPos(m_Window, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                    SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
            }
            break;
        }
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
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: {
            uint8_t key = static_cast<uint8_t>(wParam);
            const bool extended = (lParam & 0x01000000u) != 0;
            if (key == VK_CONTROL)
                key = extended ? VK_RCONTROL : VK_LCONTROL;
            else if (key == VK_SHIFT)
                key = MapVirtualKeyA((lParam & 0x00ff0000) >> 16u, MAPVK_VSC_TO_VK_EX);
            else if (key == VK_MENU)
                key = extended ? VK_RMENU : VK_LMENU;

            if (!(lParam & 0x40000000) || m_Keyboard.IsAutorepeatEnabled())
                m_Keyboard.OnKeyPressed(key);
            KeyPressedEvent e(KeyCode(key), LOWORD(lParam));
            m_Data.eventCallback(e);
            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            uint8_t key = static_cast<uint8_t>(wParam);
            const bool extended = (lParam & 0x01000000u) != 0;
            if (key == VK_CONTROL)
                key = extended ? VK_RCONTROL : VK_LCONTROL;
            else if (key == VK_SHIFT)
                key = MapVirtualKeyA((lParam & 0x00ff0000) >> 16u, MAPVK_VSC_TO_VK_EX);
            else if (key == VK_MENU)
                key = extended ? VK_RMENU : VK_LMENU;

            m_Keyboard.OnKeyReleased(key);
            KeyReleasedEvent e = { KeyCode(key) };
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
            MouseScrolledEvent e(0, static_cast<float>(delta) / static_cast<float>(WHEEL_DELTA));
            m_Data.eventCallback(e);
            break;
        }

        case WM_NCCALCSIZE: {
            if (!m_Data.titlebar && hasThickFrame && wParam) {
                const int resizeBorderX = GetSystemMetrics(SM_CXFRAME);
                const int resizeBorderY = GetSystemMetrics(SM_CYFRAME);

                NCCALCSIZE_PARAMS* params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
                RECT* clientRect = params->rgrc;

                clientRect->right -= resizeBorderX;
                clientRect->left += resizeBorderX;
                clientRect->bottom -= resizeBorderY;
                clientRect->top += 0; //1

                return WVR_ALIGNLEFT | WVR_ALIGNTOP;
            }
            break;
        }
        case WM_SIZE: {
            const int width = LOWORD(lParam);
            const int height = HIWORD(lParam);
            m_Data.minimized = wParam == SIZE_MINIMIZED;
            m_Data.maximized = wParam == SIZE_MAXIMIZED || (wParam != SIZE_RESTORED && m_Data.maximized);
            if (m_Data.width != width || m_Data.height != height) {
                m_Data.width = width;
                m_Data.height = height;

                Application::Get().GetGraphicsContext()->RecreateSwapChain(this);

                WindowResizeEvent e(width, height);
                m_Data.eventCallback(e);
            }

            RECT rect;
            GetWindowRect(m_Window, &rect);
            SetWindowPos(m_Window, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
            return 0;
        }
        case WM_MOVE: {
            int xpos = static_cast<int>(static_cast<short>(LOWORD(lParam)));
            int ypos = static_cast<int>(static_cast<short>(HIWORD(lParam)));
            m_Data.pos = { xpos, ypos };
            WindowMovedEvent e(xpos, ypos);
            m_Data.eventCallback(e);
            return 0;
        }

        case WM_ACTIVATE: {
            if (!m_Data.titlebar) {
                RECT titlebarRect = { 0 };
                InvalidateRect(m_Window, &titlebarRect, FALSE);
            }
            auto ret = HitTest(hasThickFrame, lParam);
            if (ret.has_value())
                return *ret;
            break;
        }
        case WM_NCHITTEST: {
            auto ret = HitTest(hasThickFrame, lParam);
            if (ret.has_value())
                return *ret;
            break;
        }
    }
    return DefWindowProcA(windowHandle, msg, wParam, lParam);
}
std::optional<LRESULT> WindowsWindow::HitTest(BOOL hasThickFrame, LPARAM lParam) {
    if (!m_Data.titlebar && hasThickFrame) {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        ScreenToClient(m_Window, &pt);

        if (!m_Data.maximized) {
            RECT rect;
            GetClientRect(m_Window, &rect);

            const int verticalBorderSize = GetSystemMetrics(SM_CYFRAME);

            static RECT borderThickness = { 4, 4, 4, 4 };
            enum { left = 0x1, top = 0x2, right = 0x4, bottom = 0x8 };
            int hit = 0;
            if (pt.x <= borderThickness.left) hit |= left;
            if (pt.x >= rect.right - borderThickness.right) hit |= right;
            if (pt.y <= borderThickness.top || pt.y < verticalBorderSize) hit |= top;
            if (pt.y >= rect.bottom - borderThickness.bottom) hit |= bottom;

            if (hit & top && hit & left)     return HTTOPLEFT;
            if (hit & top && hit & right)    return HTTOPRIGHT;
            if (hit & bottom && hit & left)  return HTBOTTOMLEFT;
            if (hit & bottom && hit & right) return HTBOTTOMRIGHT;
            if (hit & left)                  return HTLEFT;
            if (hit & top)                   return HTTOP;
            if (hit & right)                 return HTRIGHT;
            if (hit & bottom)                return HTBOTTOM;
        }

        int titlebarHitTest = 0;
        if (m_Data.titleBarHitTest) m_Data.titleBarHitTest(pt.x, pt.y, titlebarHitTest);
        if (titlebarHitTest) return HTCAPTION;

        return HTCLIENT;
    }
    return {};
}

WindowsWindow::WindowClass WindowsWindow::WindowClass::s_WindowClass;
WindowsWindow::WindowClass::WindowClass() noexcept
    : m_Instance(GetModuleHandle(nullptr))
{
    WNDCLASSEXA windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEXA);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowsWindow::HandleMsgSetup;
    windowClass.hInstance = m_Instance;
    windowClass.hIcon = reinterpret_cast<HICON>(LoadImageA(nullptr, (LPCSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
    windowClass.hCursor = LoadCursorA(nullptr, (LPCSTR)IDC_ARROW);
    windowClass.lpszClassName = GetName();

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
