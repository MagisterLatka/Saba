#pragma once

#include "Saba/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Saba {

class OpenGLContext : public GraphicsContext {
public:
    OpenGLContext() = default;
    ~OpenGLContext() = default;

    void Init() override {}
    void Shutdown() override;
    void InitForWindow(Window* window) override;
    void ShutdownForWindow(Window* window) override;

    void SwapBuffers(Window* window) override;
    void RecreateSwapChain([[maybe_unused]] Window *window) override {}
    void BindWindow(Window* window) override;
    void BindToRender(Window* window) override;
    void Clear(Window *window, const glm::vec4 &color) override;
private:
#if defined(SB_PLATFORM_WINDOWS)
    int (__stdcall *m_WGLSwapInternalEXT)(int) = nullptr;
    bool m_SwapControlEnabled = false;
#endif
};

} //namespace Saba
