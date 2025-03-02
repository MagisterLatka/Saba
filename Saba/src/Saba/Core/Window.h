#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Events/Event.h"
#include "Saba/Core/Keyboard.h"
#include "Saba/Core/Mouse.h"

namespace Saba {

struct WindowProps {
    std::string Title;
    uint32_t Width, Height;
    bool Resizable = true, Maximized = false, HasTitleBar = true;

    SB_CORE WindowProps() = default;
    SB_CORE WindowProps(std::string title, uint32_t width, uint32_t height,
        bool resizable = true, bool maximized = false, bool hasTitleBar = true)
        : Title(std::move(title)), Width(width), Height(height), Resizable(resizable), Maximized(maximized), HasTitleBar(hasTitleBar) {}
};

class Window : public RefCounted {
public:
    using EventCallbackFn = std::function<void(Event&)>;
    using TitleBarHitTestCallbackFn = std::function<void(const int xMousePos, const int yMousePos, int& hitTest)>;

    SB_CORE virtual ~Window() = default;

    SB_CORE virtual void OnUpdate() = 0;
    SB_CORE virtual void BindWindow() noexcept = 0;
    SB_CORE virtual void BindToRender() noexcept = 0;
    SB_CORE virtual void Clear(const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f }) noexcept = 0;
    SB_CORE static std::optional<int> ProcessEvents();

    SB_CORE virtual void SetTitle(const std::string& title) = 0;
    SB_CORE virtual const std::string& GetTitle() const noexcept = 0;

    SB_CORE virtual void SetEventCallback(EventCallbackFn callback) noexcept = 0;
    SB_CORE virtual void SetTitleBarHitTestCallback(TitleBarHitTestCallbackFn callback) noexcept = 0;

    SB_CORE virtual void EnableVSync() noexcept = 0;
    SB_CORE virtual void DisableVSync() noexcept = 0;
    SB_CORE virtual bool IsVSyncEnabled() const noexcept = 0;

    SB_CORE virtual void Minimize() noexcept = 0;
    SB_CORE virtual bool IsMinimized() const noexcept = 0;
    SB_CORE virtual void Maximize() noexcept = 0;
    SB_CORE virtual void Restore() noexcept = 0;
    SB_CORE virtual bool IsMaximized() const noexcept = 0;

    SB_CORE virtual void SetIcon(const std::filesystem::path& iconPath) = 0;

    SB_CORE virtual uint32_t GetWidth() const noexcept = 0;
    SB_CORE virtual uint32_t GetHeight() const noexcept = 0;

    SB_CORE virtual int GetXClientPos() const noexcept = 0;
    SB_CORE virtual int GetYClientPos() const noexcept = 0;

    SB_CORE virtual const Keyboard& GetKeyboard() const noexcept = 0;
    SB_CORE virtual const Mouse& GetMouse() const noexcept = 0;

    SB_CORE static Ref<Window> Create(const WindowProps& props = {});
};

} //namespace Saba
