#include <pch.h>
#include "Camera.h"

#include "Saba/Renderer/RendererAPI.h"

namespace Saba {

OrthographicCamera::OrthographicCamera(uint32_t width, uint32_t height, float nearClip, float farClip) {
    m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
    m_NearClip = nearClip;
    m_FarClip = farClip;
    Recalc();
}
OrthographicCamera::OrthographicCamera(float aspectRatio, float nearClip, float farClip) {
    m_AspectRatio = aspectRatio;
    m_NearClip = nearClip;
    m_FarClip = farClip;
    Recalc();
}

void OrthographicCamera::SetViewportSize(uint32_t width, uint32_t height) {
    m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
    Recalc();
}
void OrthographicCamera::SetAspectRatio(float aspectRatio) {
    m_AspectRatio = aspectRatio;
    Recalc();
}
void OrthographicCamera::SetSize(float size) {
    m_Size = size;
    Recalc();
}
void OrthographicCamera::SetZClips(float nearClip, float farClip) {
    m_NearClip = nearClip;
    m_FarClip = farClip;
    Recalc();
}

void OrthographicCamera::Recalc() {
    switch (RendererAPI::GetAPI()) {
        default: break;
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return;
        case RendererAPI::API::OpenGL:
            m_ProjMat = glm::orthoRH_NO(-m_AspectRatio * m_Size * 0.5f, m_AspectRatio * m_Size * 0.5f, -m_Size * 0.5f, m_Size * 0.5f, m_NearClip, m_FarClip);
            return;
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:
        case RendererAPI::API::Vulkan:
            m_ProjMat = glm::orthoRH_ZO(-m_AspectRatio * m_Size * 0.5f, m_AspectRatio * m_Size * 0.5f, -m_Size * 0.5f, m_Size * 0.5f, m_NearClip, m_FarClip);
            return;
    }
    SB_CORE_THROW_INFO("Unknown API");
}



PerspectiveCamera::PerspectiveCamera(uint32_t width, uint32_t height, float fov, float nearClip, float farClip)
    : m_Fov(fov)
{
    m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
    m_NearClip = nearClip;
    m_FarClip = farClip;
    Recalc();
}
PerspectiveCamera::PerspectiveCamera(float aspectRatio, float fov, float nearClip, float farClip)
    : m_Fov(fov)
{
    m_AspectRatio = aspectRatio;
    m_NearClip = nearClip;
    m_FarClip = farClip;
    Recalc();
}

void PerspectiveCamera::SetViewportSize(uint32_t width, uint32_t height) {
    m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
    Recalc();
}
void PerspectiveCamera::SetAspectRatio(float aspectRatio) {
    m_AspectRatio = aspectRatio;
    Recalc();
}
void PerspectiveCamera::SetFOV(float fov) {
    m_Fov = fov;
    Recalc();
}
void PerspectiveCamera::SetZClips(float nearClip, float farClip) {
    m_NearClip = nearClip;
    m_FarClip = farClip;
    Recalc();
}

void PerspectiveCamera::Recalc() {
    switch (RendererAPI::GetAPI()) {
        default: break;
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return;
        case RendererAPI::API::OpenGL:
            m_ProjMat = glm::perspectiveRH_NO(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
            return;
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:
        case RendererAPI::API::Vulkan:
            m_ProjMat = glm::perspectiveRH_ZO(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);;
            return;
    }
    SB_CORE_THROW_INFO("Unknown API");
}

}
