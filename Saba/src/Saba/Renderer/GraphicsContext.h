#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Core/Window.h"

namespace Saba {

class GraphicsContext : public RefCounted {
    friend class Application;
    friend class LinuxWindow;
    friend class WindowsWindow;
public:
    SB_CORE virtual ~GraphicsContext() = default;
protected:
    SB_CORE virtual void Init() = 0;
    SB_CORE virtual void Shutdown() = 0;
    SB_CORE virtual void InitForWindow(Window* window) = 0;
    SB_CORE virtual void ShutdownForWindow(Window* window) = 0;

    SB_CORE virtual void SwapBuffers(Window* window) = 0;
    SB_CORE virtual void RecreateSwapChain(Window* window) = 0;
    SB_CORE virtual void BindWindow(Window* window) = 0;
    SB_CORE virtual void BindToRender(Window* window) = 0;
    SB_CORE virtual void Clear(Window* window, const glm::vec4& color) = 0;

    SB_CORE static Ref<GraphicsContext> Create();
};

}
