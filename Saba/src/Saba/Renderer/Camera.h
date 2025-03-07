#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class Camera : public RefCounted {
public:
    SB_CORE Camera() = default;
    SB_CORE Camera(glm::mat4 projectionMatrix) noexcept
        : m_ProjMat(projectionMatrix) {}
    SB_CORE virtual ~Camera() = default;

    SB_CORE virtual void SetViewportSize(uint32_t width, uint32_t height) = 0;

    SB_CORE const glm::mat4& GetProjectionMatrix() const noexcept { return m_ProjMat; }
protected:
    glm::mat4 m_ProjMat = glm::mat4(1.0f);
};

class OrthographicCamera : public Camera {
public:
    SB_CORE OrthographicCamera() = default;
    SB_CORE OrthographicCamera(glm::mat4 projectionMatrix) noexcept
        : Camera(projectionMatrix) {}
    SB_CORE OrthographicCamera(uint32_t width, uint32_t height, float nearClip = -1.0f, float farClip = 1.0f);
    SB_CORE OrthographicCamera(float aspectRatio, float nearClip = -1.0f, float farClip = 1.0f);
    SB_CORE ~OrthographicCamera() = default;

    SB_CORE void SetViewportSize(uint32_t width, uint32_t height) override;
    SB_CORE void SetAspectRatio(float aspectRatio);
    SB_CORE float GetAspectRatio() const noexcept { return m_AspectRatio; }

    SB_CORE void SetSize(float size);
    SB_CORE float GetSize() const noexcept { return m_Size; }
    SB_CORE float GetWidth() const noexcept { return m_Size * m_AspectRatio; }
    SB_CORE float GetHeight() const noexcept { return m_Size; }

    SB_CORE void SetZClips(float nearClip, float farClip);
    SB_CORE float GetNearClip() const noexcept { return m_NearClip;}
    SB_CORE float GetFarClip() const noexcept { return m_FarClip;}
private:
    void Recalc();
private:
    float m_AspectRatio = 1.0f, m_Size = 1.0f, m_NearClip = -1.0f, m_FarClip = 1.0f;
};

}