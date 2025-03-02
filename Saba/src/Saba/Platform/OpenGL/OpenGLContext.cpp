#include <pch.h>
#include "OpenGLContext.h"

#if defined(SB_PLATFORM_WINDOWS)
#   include "Saba/Platform/Windows/WindowsWindow.h"

#   define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#   define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#   define WGL_CONTEXT_FLAGS_ARB 0x2094
#   define WGL_CONTEXT_COREPROFILE_BIT_ARB 0x00000001
#   define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);
typedef const char* (WINAPI* PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);
typedef int (WINAPI* PFNWGLGETSWAPINTERVALEXTPROC) (void);
#elif defined(SB_PLATFORM_LINUX)
#   include "Saba/Platform/Linux/LinuxWindow.h"
#endif
#include <glad/glad.h>

#include "Saba/Renderer/Renderer.h"

namespace Saba {

void OpenGLContext::Shutdown() {
#if !defined(SB_PLATFORM_WINDOWS)
    glfwTerminate();
#endif
}

void OpenGLContext::InitForWindow([[maybe_unused]] Window* window) {
#if defined(SB_PLATFORM_WINDOWS)
    auto* wnd = reinterpret_cast<WindowsWindow*>(window);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(wnd->m_DC, &pfd);
    if (pixelFormat == 0)
        throw SB_WINDOWS_WINDOW_LAST_EXCEPTION();
    SetPixelFormat(wnd->m_DC, pixelFormat, &pfd);

    HGLRC tempRC = wglCreateContext(wnd->m_DC);
    wglMakeCurrent(wnd->m_DC, tempRC);
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    const int attribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_FLAGS_ARB, 0,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_COREPROFILE_BIT_ARB, 0,
    };

    wnd->m_Context = wglCreateContextAttribsARB(wnd->m_DC, 0, attribList);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempRC);
    wglMakeCurrent(wnd->m_DC, wnd->m_Context);

    int success = gladLoadGL();
    SB_CORE_ASSERT(success, "Failed to initialize GLAD");

    static bool initialized = false;
    if (!initialized) {
        SB_CORE_INFO("OpenGL Info:");
        SB_CORE_INFO("\tRenderer: {0}", (char*)glGetString(GL_RENDERER));
        SB_CORE_INFO("\tVersion: {0}", (char*)glGetString(GL_VERSION));
        initialized = true;
    }

    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
    if ((m_SwapControlEnabled = strstr(_wglGetExtensionsStringEXT(), "WGL_EXT_swap_control")) != 0) {
        m_WGLSwapInternalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        m_WGLSwapInternalEXT(0);
        //PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
    }
#elif defined(SB_PLATFORM_LINUX)
    int success = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    SB_CORE_ASSERT(success, "Failed to initialize GLAD");

    SB_CORE_INFO("OpenGL info:");
    SB_CORE_INFO("\tRenderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    SB_CORE_INFO("\tVersion: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
#endif
}
void OpenGLContext::ShutdownForWindow([[maybe_unused]] Window* window)
{
#if defined(SB_PLATFORM_WINDOWS)
    auto* wnd = reinterpret_cast<WindowsWindow*>(window);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(wnd->m_Context);
    ReleaseDC(wnd->m_Window, wnd->m_DC);
#endif
}

void OpenGLContext::SwapBuffers(Window* window)
{
#if defined(SB_PLATFORM_WINDOWS)
    auto* wnd = reinterpret_cast<WindowsWindow*>(window);

    static bool vsync = false;
    if (vsync != wnd->m_Data.vSync) {
        vsync = wnd->m_Data.vSync;
        m_WGLSwapInternalEXT(vsync ? 1 : 0);
    }

    ::SwapBuffers(wnd->m_DC);

    if (vsync)
        glFinish();
#else
    glfwSwapBuffers(reinterpret_cast<LinuxWindow*>(window)->m_Window);
#endif
}

void OpenGLContext::BindWindow(Window* window)
{
#if defined(SB_PLATFORM_WINDOWS)
    auto* wnd = reinterpret_cast<WindowsWindow*>(window);
    wglMakeCurrent(wnd->m_DC, wnd->m_Context);
#else
    glfwMakeContextCurrent(reinterpret_cast<LinuxWindow*>(window)->m_Window);
#endif
}
void OpenGLContext::BindToRender(Window* window) {
    Renderer::Submit([window]() {
#if defined(SB_PLATFORM_WINDOWS)
        auto* wnd = reinterpret_cast<WindowsWindow*>(window);
#else
        auto* wnd = reinterpret_cast<LinuxWindow*>(window);
#endif
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, static_cast<int>(wnd->m_Data.width), static_cast<int>(wnd->m_Data.height));
    });
}

void OpenGLContext::Clear([[maybe_unused]] Window* window, const glm::vec4& color) {
    Renderer::Submit([color]() {
        glClearColor(color.x, color.y, color.r, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    });
}

}
