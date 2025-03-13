#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Events/MouseEvents.h"

namespace Saba {

class EditorCamera : public RefCounted {
public:
    SB_CORE EditorCamera() = default;
    SB_CORE EditorCamera(float fov, float aspectRatio, float nearClip = 0.1f, float farClip = 100.0f);

    SB_CORE void OnUpdate();
    SB_CORE void OnEvent(Event& e);

    SB_CORE void SetViewportSize(uint32_t width, uint32_t height);
    SB_CORE float GetDistance() const noexcept { return m_Distance; }
    SB_CORE float& GetDistance() noexcept { return m_Distance; }
    SB_CORE const glm::vec3& GetPosition() const noexcept { return m_Pos; }
    SB_CORE glm::vec3& GetPosition() noexcept { return m_Pos; };
    SB_CORE float GetPitch() const noexcept { return m_Pitch; }
    SB_CORE float GetYaw() const noexcept { return m_Yaw; }

    SB_CORE const glm::mat4& GetProjectionMatrix() const noexcept { return m_ProjMat; }
    SB_CORE const glm::mat4& GetViewMatrix() const noexcept { return m_ViewMat; }
    SB_CORE glm::mat4 GetProjectionViewMatrix() const noexcept { return m_ProjMat * m_ViewMat; }
private:
    void UpdateProj();
    void UpdateView();
    glm::quat GetOrientation() const noexcept;

    bool OnMouseScrolled(MouseScrolledEvent& e);
private:
    glm::mat4 m_ProjMat = glm::mat4(1.0f), m_ViewMat = glm::mat4(1.0f);
    glm::vec3 m_Pos = { 0.0f, 0.0f, 0.0f }, m_FocalPoint = { 0.0f, 0.0f, 0.0f };
    glm::vec2 m_MousePos { 0.0f, 0.0f };
    float m_FOV = glm::half_pi<float>(), m_AspectRatio = 16.0f / 9.0f, m_NearClip = 0.1f, m_FarClip = 100.0f;
    float m_Distance = 5.0f, m_Pitch = 0.0f, m_Yaw = 0.0f;
    uint32_t m_ViewportWidth = 1280u, m_ViewportHeight = 720u;
};

}
