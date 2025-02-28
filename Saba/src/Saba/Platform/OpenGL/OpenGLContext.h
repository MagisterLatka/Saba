#pragma once

#include "Saba/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Saba {

class OpenGLContext : public GraphicsContext {
public:
    SB_CORE OpenGLContext() = default;
    SB_CORE ~OpenGLContext() = default;

    SB_CORE void Init() override {}
    SB_CORE void Shutdown() override;
    SB_CORE void InitForWindow(void* window) override;
    SB_CORE void ShutdownForWindow(void* window) override;

    SB_CORE void SwapBuffers(void* window) override;
    SB_CORE void BindWindow(void* window) override;
    SB_CORE void BindToRender(void* window) override;
    SB_CORE void Clear(void *window, const glm::vec4 &color) override;
private:
#if defined(SB_PLATFORM_WINDOWS)
    int (__stdcall *m_WGLSwapInternalEXT)(int) = nullptr;
    bool m_SwapControlEnabled = false;
#endif
};

}
