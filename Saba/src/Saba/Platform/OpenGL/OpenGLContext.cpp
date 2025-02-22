#include <pch.h>
#include "OpenGLContext.h"

#include <glad/glad.h>

namespace Saba {

OpenGLContext::OpenGLContext(void* window)
    : m_Window(reinterpret_cast<GLFWwindow*>(window)) {}

void OpenGLContext::InitWindow() {
    int success = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    SB_CORE_ASSERT(success, "Failed to initialize GLAD");

    SB_CORE_INFO("OpenGL info:");
    SB_CORE_INFO("\tRenderer: {0}", (char*)glGetString(GL_RENDERER));
    SB_CORE_INFO("\tVersion: {0}", (char*)glGetString(GL_VERSION));
}

}
