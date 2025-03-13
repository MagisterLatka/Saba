#include <pch.h>
#include "EditorCamera.h"

#include "Saba/Renderer/RendererAPI.h"
#include "Saba/Core/Input.h"

namespace Saba {

static glm::vec3 GetUpDirection(glm::quat quat) noexcept {
    return glm::rotate(quat, glm::vec3(0.0f, 1.0f, 0.0f));
}
static glm::vec3 GetRightDirection(glm::quat quat) noexcept {
    return glm::rotate(quat, glm::vec3(1.0f, 0.0f, 0.0f));
}
static glm::vec3 GetForwardDirection(glm::quat quat) noexcept {
    return glm::rotate(quat, glm::vec3(0.0f, 0.0f, -1.0f));
}
static glm::vec2 GetPanSpeed(uint32_t viewportWidth, uint32_t viewportHeight) noexcept {
    const float a = 0.0366f, b = 0.1778f, c = 0.3021f;
    const float x = glm::min(static_cast<float>(viewportWidth) / 1000.0f, 2.4f);
    const float xSpeed = a * x * x - b * x + c;
    const float y = glm::min(static_cast<float>(viewportHeight) / 1000.0f, 2.4f);
    const float ySpeed = a * y * y - b * y + c;
    return { xSpeed, ySpeed };
}
static float GetZoomSpeed(float distance) noexcept {
    float d = glm::max(distance * 0.2f, 0.0f);
    return glm::min(glm::pow(d, 1.2f), 10.0f);
}

EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
    : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
{
    UpdateProj();
    UpdateView();
}

void EditorCamera::OnUpdate() {
    if (Input::IsKeyPressed(KeyCode::LeftAlt)) {
        glm::vec2 delta = (Input::GetMousePos() - m_MousePos) * 0.003f;
        m_MousePos = Input::GetMousePos();
        glm::quat orientation = GetOrientation();

        if (Input::IsMouseButtonPressed(MouseCode::ButtonLeft)) {
            static constexpr float rotateSpeed = 0.8f;
            m_Yaw += (GetUpDirection(orientation).y < 0.0f ? -1.0f : 1.0f) * delta.x * rotateSpeed;
            m_Pitch += delta.y * rotateSpeed;
        }
        else if (Input::IsMouseButtonPressed(MouseCode::ButtonRight)) {
            m_Distance = glm::max(1.0f, m_Distance - delta.y * GetZoomSpeed(m_Distance));
        }
        else if (Input::IsMouseButtonPressed(MouseCode::ButtonMiddle)) {
            glm::vec2 panSpeed = GetPanSpeed(m_ViewportWidth, m_ViewportHeight);
            m_FocalPoint += -GetRightDirection(orientation) * delta.x * panSpeed.x * m_Distance;
            m_FocalPoint += GetUpDirection(orientation) * delta.y * panSpeed.y * m_Distance;
        }
    }
    UpdateView();
}
void EditorCamera::OnEvent(Event& e) {
    Dispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(SB_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
}
bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e) {
    m_Distance = glm::max(1.0f, m_Distance - e.GetYOffset() * 0.1f * GetZoomSpeed(m_Distance));
    return false;
}

void EditorCamera::UpdateProj() {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return;
        case RendererAPI::API::OpenGL:
            m_ProjMat = glm::perspectiveRH_NO(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
            return;
        case RendererAPI::API::Vulkan:
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:
            m_ProjMat = glm::perspectiveRH_ZO(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
            return;
    }
    SB_CORE_THROW_INFO("Unknown API");
}
void EditorCamera::UpdateView() {
    glm::quat orientation = GetOrientation();
    m_Pos = m_FocalPoint - GetForwardDirection(orientation) * m_Distance;
    m_ViewMat = glm::inverse(glm::translate(glm::mat4(1.0f), m_Pos) * glm::toMat4(orientation));
}

glm::quat EditorCamera::GetOrientation() const noexcept {
    return { glm::vec3(-m_Pitch, -m_Yaw, 0.0f) };
}

void EditorCamera::SetViewportSize(uint32_t width, uint32_t height) {
    m_ViewportWidth = width;
    m_ViewportHeight = height;
    m_AspectRatio = static_cast<float>(m_ViewportWidth) / static_cast<float>(m_ViewportHeight);
    UpdateProj();
}

}
