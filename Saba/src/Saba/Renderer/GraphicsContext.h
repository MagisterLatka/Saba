#pragma once

#include "Saba/Core/Window.h"

namespace Saba {

class GraphicsContext : public RefCounted {
    friend class Application;
    friend class LinuxWindow;
    friend class WindowsWindow;
public:
    virtual ~GraphicsContext() = default;
protected:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void InitForWindow(Window* window) = 0;
    virtual void ShutdownForWindow(Window* window) = 0;

    virtual void SwapBuffers(Window* window) = 0;
    virtual void RecreateSwapChain(Window* window) = 0;
    virtual void BindWindow(Window* window) = 0;
    virtual void BindToRender(Window* window) = 0;
    virtual void Clear(Window* window, const glm::vec4& color) = 0;

    static Ref<GraphicsContext> Create();
};

} //namespace Saba
