#include <pch.h>
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Saba {

static void OpenGLMessageCallback(GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id,
    GLenum severity, [[maybe_unused]] GLsizei length, const GLchar* message, [[maybe_unused]] const void* userParam)
{
    const char* Source;

    switch (source) {
        case GL_DEBUG_SOURCE_API:               Source = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     Source = "WINDOW SYSTEM"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   Source = "SHADER COMPILER"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       Source = "THIRD PARTY"; break;
        case GL_DEBUG_SOURCE_APPLICATION:       Source = "APPLICATION"; break;
        case GL_DEBUG_SOURCE_OTHER:
        default:                                Source = "UNKNOWN"; break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:            SB_CORE_CRITICAL("({0}): {1}", Source, message); break;
        case GL_DEBUG_SEVERITY_MEDIUM:          SB_CORE_ERROR("({0}): {1}", Source, message); break;
        case GL_DEBUG_SEVERITY_LOW:             SB_CORE_WARN("({0}): {1}", Source, message); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        default:                                SB_CORE_TRACE("({0}): {1}", Source, message); break;
    }
}

void OpenGLRendererAPI::Init() {
#if defined(SB_DEBUG)
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif
}
void OpenGLRendererAPI::Shutdown() {}

void OpenGLRendererAPI::Draw(Topology topology, uint32_t verticesCount) {
    glDrawArrays(static_cast<GLenum>(GetTopology(topology)), 0, static_cast<int>(verticesCount));
}
void OpenGLRendererAPI::DrawIndexed(Topology topology, uint32_t indicesCount) {
    glDrawElements(static_cast<GLenum>(GetTopology(topology)), static_cast<int>(indicesCount), GL_UNSIGNED_INT, nullptr);
}

int OpenGLRendererAPI::GetTopology(Topology topology) {
    switch (topology) {
        case RendererAPI::Topology::None:           SB_CORE_THROW_INFO("None topology cannot be used"); return -1;
        case RendererAPI::Topology::Points:         return GL_POINTS;
        case RendererAPI::Topology::Lines:          return GL_LINES;
        case RendererAPI::Topology::LineStrip:      return GL_LINE_STRIP;
        case RendererAPI::Topology::Triangles:      return GL_TRIANGLES;
        case RendererAPI::Topology::TriangleStrip:  return GL_TRIANGLE_STRIP;
    }
    SB_CORE_THROW_INFO("Unknown topology");
    return -1;
}

}
