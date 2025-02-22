#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class RendererAPI {
    friend class Application;
public:
    enum class API : uint8_t { None = 0, OpenGL, DX11, DX12, Vulkan };
public:
    SB_CORE static API GetAPI() { return s_API; }
private:
    SB_CORE static API s_API;
};

}
