#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class Camera : public RefCounted {
public:
    Camera() = default;
    Camera(glm::mat4 projectionMatrix) noexcept
        : m_ProjMat(projectionMatrix) {}
    virtual ~Camera() = default;

    virtual void SetViewportSize(uint32_t width, uint32_t height) = 0;
    virtual void SetAspectRatio(float aspectRatio) = 0;
    float GetAspectRatio() const noexcept { return m_AspectRatio; }

    virtual void SetZClips(float nearClip, float farClip) = 0;
    float GetNearClip() const noexcept { return m_NearClip; }
    float GetFarClip() const noexcept { return m_FarClip; }

    const glm::mat4& GetProjectionMatrix() const noexcept { return m_ProjMat; }
protected:
    glm::mat4 m_ProjMat = glm::mat4(1.0f);
    float m_AspectRatio = 1.0f, m_NearClip = -1.0f, m_FarClip = 1.0f;
};

class OrthographicCamera : public Camera {
    friend class SceneHierarchyPanel;
public:
    OrthographicCamera() = default;
    OrthographicCamera(glm::mat4 projectionMatrix) noexcept
        : Camera(projectionMatrix) {}
    SB_CORE OrthographicCamera(uint32_t width, uint32_t height, float nearClip = -1.0f, float farClip = 1.0f);
    SB_CORE OrthographicCamera(float aspectRatio, float nearClip = -1.0f, float farClip = 1.0f);
    ~OrthographicCamera() = default;

    SB_CORE void SetViewportSize(uint32_t width, uint32_t height) override;
    SB_CORE void SetAspectRatio(float aspectRatio) override;
    SB_CORE void SetZClips(float nearClip, float farClip) override;

    SB_CORE void SetSize(float size);
    float GetSize() const noexcept { return m_Size; }
    float GetWidth() const noexcept { return m_Size * m_AspectRatio; }
    float GetHeight() const noexcept { return m_Size; }
private:
    void Recalc();
private:
    float m_Size = 1.0f;
};

class PerspectiveCamera : public Camera {
    friend class SceneHierarchyPanel;
public:
    PerspectiveCamera() = default;
    PerspectiveCamera(glm::mat4 projectionMatrix) noexcept
        : Camera(projectionMatrix) {}
    SB_CORE PerspectiveCamera(uint32_t width, uint32_t height, float fov = glm::half_pi<float>(), float nearClip = 0.1f, float farClip = 100.0f);
    SB_CORE PerspectiveCamera(float aspectRatio, float fov = glm::half_pi<float>(), float nearClip = 0.1f, float farClip = 100.0f);
    ~PerspectiveCamera() = default;

    SB_CORE void SetViewportSize(uint32_t width, uint32_t height) override;
    SB_CORE void SetAspectRatio(float aspectRatio) override;
    SB_CORE void SetZClips(float nearClip, float farClip) override;

    SB_CORE void SetFOV(float fov);
    float GetFOV() const noexcept { return m_Fov; }
private:
    void Recalc();
private:
    float m_Fov = glm::half_pi<float>();
};

} //namespace Saba
