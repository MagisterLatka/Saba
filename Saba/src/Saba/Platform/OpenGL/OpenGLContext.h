#pragma once

#include "Saba/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Saba {

class OpenGLContext : public GraphicsContext {
public:
    SB_CORE OpenGLContext(void* window);
    SB_CORE ~OpenGLContext() = default;

    SB_CORE void Init() override {}
    SB_CORE void InitWindow();
private:
    GLFWwindow* m_Window = nullptr;
};

}
