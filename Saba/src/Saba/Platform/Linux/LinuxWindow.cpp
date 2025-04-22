#include <pch.h>
#include "LinuxWindow.h"

#include "Saba/Core/Application.h"

#include "Saba/Events/ApplicationEvents.h"
#include "Saba/Events/KeyEvents.h"
#include "Saba/Events/MouseEvents.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Saba {

LinuxWindow::LinuxWindow(const WindowProps& props) {
    Init(props);
}
LinuxWindow::~LinuxWindow() {
    Shutdown();
}

void LinuxWindow::OnUpdate() {
    glfwSwapBuffers(m_Window);
}
std::optional<int> LinuxWindow::ProcessEvents() {
    glfwPollEvents();
    return {};
}
void LinuxWindow::BindWindow() noexcept {
    Application::Get().GetGraphicsContext()->BindWindow(this);
}
void LinuxWindow::BindToRender() noexcept {
    Application::Get().GetGraphicsContext()->BindToRender(this);
}
void LinuxWindow::Clear(const glm::vec4& color) noexcept {
    Application::Get().GetGraphicsContext()->Clear(this, color);
}

void LinuxWindow::SetTitle(const std::string& title) {
    glfwSetWindowTitle(m_Window, title.c_str());
    m_Data.title = title;
}
void LinuxWindow::EnableVSync() noexcept {
    glfwSwapInterval(1);
    m_Data.vSync = true;
}
void LinuxWindow::DisableVSync() noexcept {
    glfwSwapInterval(0);
    m_Data.vSync = false;
}

void LinuxWindow::Minimize() noexcept {
    m_Data.minimized = true;
    glfwIconifyWindow(m_Window);
}
void LinuxWindow::Maximize() noexcept {
    m_Data.maximized = true;
    glfwMaximizeWindow(m_Window);
}
void LinuxWindow::Restore() noexcept {
    m_Data.maximized = false;
    glfwRestoreWindow(m_Window);
}

void LinuxWindow::SetIcon(const std::filesystem::path& iconPath) {
    std::string path = iconPath.string();
    if (std::filesystem::exists(iconPath)) {
        GLFWimage image;
        int channels;
        image.pixels = stbi_load(path.c_str(), &image.width, &image.height, &channels, 4);
        glfwSetWindowIcon(m_Window, 1, &image);
        stbi_image_free(image.pixels);
    }
}

SB_CORE static void GLFWErrorCallback(int error, const char* message) {
    SB_CORE_ERROR("GLFWError {0} {1}", error, message);
}
void LinuxWindow::Init(const WindowProps& props) {
    m_Data.title = props.Title;
    m_Data.width = props.Width;
    m_Data.height = props.Height;
    m_Data.titlebar = props.HasTitleBar;
    m_Data.maximized = props.Maximized;
    m_Data.eventCallback = SB_BIND_EVENT_FN(LinuxWindow::DefaultEventCallback);

    SB_CORE_ASSERT(glfwInit(), "Failed to initialize GLFW");
    glfwSetErrorCallback(GLFWErrorCallback);

    glfwWindowHint(GLFW_RESIZABLE, static_cast<int>(props.Resizable));
    glfwWindowHint(GLFW_DECORATED, static_cast<int>(props.HasTitleBar));
    glfwWindowHint(GLFW_MAXIMIZED, static_cast<int>(props.Maximized));

    m_Window = glfwCreateWindow(static_cast<int>(m_Data.width), static_cast<int>(m_Data.height), m_Data.title.c_str(), nullptr, nullptr);
    SB_CORE_ASSERT(m_Window, "Could not create GLFW window");
    SB_CORE_INFO("Creating window {0}, ({1}, {2})", m_Data.title, m_Data.width, m_Data.height);

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    glfwSwapInterval(m_Data.vSync ? 1 : 0);

    Application::Get().GetGraphicsContext()->InitForWindow(this);

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent e;
        data.eventCallback(e);
    });
    glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y) {
        auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.pos = { x, y };

        WindowMovedEvent e(x, y);
        data.eventCallback(e);
    });
    glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
        auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
        if (focused == GLFW_TRUE) {
            WindowFocusEvent e;
            data.eventCallback(e);
        }
        else {
            WindowLostFocusEvent e;
            data.eventCallback(e);
        }
    });
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.width = static_cast<uint32_t>(width);
        data.height = static_cast<uint32_t>(height);

        WindowResizeEvent e(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
        data.eventCallback(e);
    });
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
        auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
        KeyCode code = GetKeyCodeFromGLFW(key);
        switch (action) {
            case GLFW_PRESS: {
                data.keyboard.OnKeyPressed(static_cast<uint8_t>(code));
                KeyPressedEvent e(code, 0);
                data.eventCallback(e);
                break;
            }
            case GLFW_REPEAT: {
                data.keyboard.OnKeyPressed(static_cast<uint8_t>(code));
                KeyPressedEvent e(code, 1);
                data.eventCallback(e);
                break;
                break;
            }
            case GLFW_RELEASE: {
                data.keyboard.OnKeyReleased(static_cast<uint8_t>(code));
                KeyReleasedEvent e(code);
                data.eventCallback(e);
                break;
            }
            default:
                throw;
        }
    });
    glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t code) {
        auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.keyboard.OnChar(static_cast<char>(code));
        KeyTypedEvent e(static_cast<uint8_t>(code));
        data.eventCallback(e);
    });
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
        auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            if (button == GLFW_MOUSE_BUTTON_LEFT)
                data.mouse.OnLeftPressed();
            else if (button == GLFW_MOUSE_BUTTON_RIGHT)
                data.mouse.OnRightPressed();

            MouseButtonPressedEvent e(static_cast<MouseCode>(button));
            data.eventCallback(e);

            if (!data.titlebar && button == GLFW_MOUSE_BUTTON_LEFT) {
                int titlebarHitTest = 0;
                glm::ivec2 mousePos = data.mouse.GetPos();
                if (data.titlebarHitTest)
                    data.titlebarHitTest(mousePos.x, mousePos.y, titlebarHitTest);

                if (titlebarHitTest)
                    glfwDragWindow(window);

                if (!data.maximized) {
                    static uint32_t borderThickness = 4;
                    enum : uint8_t { left = 0x1, top = 0x2, right = 0x4, bottom = 0x8 };
                    int hit = 0;
                    if (mousePos.x <= static_cast<int>(borderThickness)) hit |= left;
                    if (mousePos.x >= static_cast<int>(data.width - borderThickness)) hit |= right;
                    if (mousePos.y <= static_cast<int>(borderThickness)) hit |= top;
                    if (mousePos.y >= static_cast<int>(data.height - borderThickness)) hit |= bottom;

                    int border = -1;
                    if (hit & top && hit & left)            border = GLFW_WINDOW_LEFT_TOP;
                    else if (hit & top && hit & right)      border = GLFW_WINDOW_RIGHT_TOP;
                    else if (hit & bottom && hit & left)    border = GLFW_WINDOW_LEFT_BOTTOM;
                    else if (hit & bottom && hit & right)   border = GLFW_WINDOW_RIGHT_BOTTOM;
                    else if (hit & left)                    border = GLFW_WINDOW_LEFT;
                    else if (hit & top)                     border = GLFW_WINDOW_TOP;
                    else if (hit & right)                   border = GLFW_WINDOW_RIGHT;
                    else if (hit & bottom)                  border = GLFW_WINDOW_BOTTOM;

                    if (border != -1)
                        glfwResizeWindow(window, border);
                }
            }
        }
        else {
            if (button == GLFW_MOUSE_BUTTON_LEFT)
                data.mouse.OnLeftReleased();
            else if (button == GLFW_MOUSE_BUTTON_RIGHT)
                data.mouse.OnRightReleased();

            if (!data.mouse.IsInWindow()) {
                MouseLeftEvent e;
                data.eventCallback(e);
            }

            MouseButtonReleasedEvent e(static_cast<MouseCode>(button));
            data.eventCallback(e);
        }
    });
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
        auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.mouse.OnMouseMove(static_cast<int>(xpos), static_cast<int>(ypos));
        MouseMovedEvent e(static_cast<float>(xpos), static_cast<float>(ypos));
        data.eventCallback(e);
    });
    glfwSetCursorEnterCallback(m_Window, [](GLFWwindow* window, int entered) {
        auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
        if (entered) {
            data.mouse.OnMouseEnter();
            MouseEnteredEvent e;
            data.eventCallback(e);
        }
        else {
            data.mouse.OnMouseLeave();
            if (!(data.mouse.IsLeftButtonPressed() || data.mouse.IsLeftButtonPressed())) {
                MouseLeftEvent e;
                data.eventCallback(e);
            }
        }
    });
    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
        auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.mouse.OnWheelDelta(static_cast<int>(yoffset));
        MouseScrolledEvent e(static_cast<float>(xoffset), static_cast<float>(yoffset));
        data.eventCallback(e);
    });

    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    m_Data.width = static_cast<uint32_t>(width);
    m_Data.height = static_cast<uint32_t>(height);
    int xpos, ypos;
    glfwGetWindowPos(m_Window, &xpos, &ypos);
    m_Data.pos = { xpos, ypos };
}
void LinuxWindow::Shutdown() {
    glfwDestroyWindow(m_Window);
}

} //namespace Saba
