#pragma once

#include "Saba/Renderer/RenderCommandQueue.h"
#include "Saba/Renderer/Shader.h"

namespace Saba {

class Renderer {
    friend class Application;
public:
    template<typename T>
    requires std::is_invocable_v<T>
    static void Submit(T&& fn) {
        auto renderCommand = [](void* ptr) {
            auto fn = reinterpret_cast<T*>(ptr);
            (*fn)();
            fn->~T();
        };
        auto buffer = GetRenderCommandQueue().Allocate(renderCommand, sizeof(fn));
        new (buffer) T(std::forward<T>(fn));
    }
    SB_CORE static void Render();

    SB_CORE static ShaderLibrary& GetShaderLibrary() noexcept;
private:
    SB_CORE static void Init();
    SB_CORE static void Shutdown();
    SB_CORE static RenderCommandQueue& GetRenderCommandQueue() noexcept;
};

}
