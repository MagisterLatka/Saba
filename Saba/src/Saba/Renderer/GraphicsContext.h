#pragma once

#include "Saba/Core/Core.h"

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
    SB_CORE virtual void InitForWindow(void* window) = 0;
    SB_CORE virtual void ShutdownForWindow(void* window) = 0;

    SB_CORE virtual void SwapBuffers(void* window) = 0;
    SB_CORE virtual void BindWindow(void* window) = 0;
    SB_CORE virtual void BindToRender(void* window) = 0;
    SB_CORE virtual void Clear(void* window, const glm::vec4& color) = 0;

    SB_CORE static Ref<GraphicsContext> Create();
};

}
