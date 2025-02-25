#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Events/Event.h"
#include "Saba/Core/Keyboard.h"
#include "Saba/Core/Mouse.h"

namespace Saba {

struct WindowProps {
    std::string Title;
    uint32_t Width, Height;

    SB_CORE WindowProps(std::string title = "Saba Engine", uint32_t width = 1280u, uint32_t height = 720u)
        : Title(std::move(title)), Width(width), Height(height) {}
};

class Window : public RefCounted {
public:
    using EventCallbackFn = std::function<void(Event&)>;

    SB_CORE virtual ~Window() = default;

    SB_CORE virtual void OnUpdate() = 0;
    SB_CORE virtual void BindWindow() noexcept = 0;
    SB_CORE virtual void BindToRender() noexcept = 0;
    SB_CORE virtual void Clear(const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f }) noexcept = 0;
    SB_CORE static std::optional<int> ProcessEvents();

    SB_CORE virtual void SetTitle(const std::string& title) = 0;
    SB_CORE virtual const std::string& GetTitle() const noexcept = 0;

    SB_CORE virtual void SetEventCallback(EventCallbackFn callback) noexcept = 0;
    SB_CORE virtual void EnableVSync() noexcept = 0;
    SB_CORE virtual void DisableVSync() noexcept = 0;
    SB_CORE virtual bool IsVSyncEnabled() const noexcept = 0;

    SB_CORE virtual uint32_t GetWidth() const noexcept = 0;
    SB_CORE virtual uint32_t GetHeight() const noexcept = 0;

    SB_CORE virtual const Keyboard& GetKeyboard() const noexcept = 0;
    SB_CORE virtual const Mouse& GetMouse() const noexcept = 0;

    SB_CORE static Ref<Window> Create(const WindowProps& props = {});
};

} //namespace Saba
