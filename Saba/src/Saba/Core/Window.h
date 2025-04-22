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

    WindowProps() = default;
    WindowProps(std::string title, uint32_t width, uint32_t height,
        bool resizable = true, bool maximized = false, bool hasTitleBar = true)
        : Title(std::move(title)), Width(width), Height(height), Resizable(resizable), Maximized(maximized), HasTitleBar(hasTitleBar) {}
};

class Window : public RefCounted {
public:
    using EventCallbackFn = std::function<void(Event&)>;
    using TitleBarHitTestCallbackFn = std::function<void(const int xMousePos, const int yMousePos, int& hitTest)>;

    SB_CORE virtual ~Window() = default;

    virtual void OnUpdate() = 0;
    virtual void BindWindow() noexcept = 0;
    virtual void BindToRender() noexcept = 0;
    virtual void Clear(const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f }) noexcept = 0;
    SB_CORE static std::optional<int> ProcessEvents();

    virtual void SetTitle(const std::string& title) = 0;
    virtual const std::string& GetTitle() const noexcept = 0;

    virtual void SetEventCallback(EventCallbackFn callback) noexcept = 0;
    virtual void SetTitleBarHitTestCallback(TitleBarHitTestCallbackFn callback) noexcept = 0;

    virtual void EnableVSync() noexcept = 0;
    virtual void DisableVSync() noexcept = 0;
    virtual bool IsVSyncEnabled() const noexcept = 0;

    virtual void Minimize() noexcept = 0;
    virtual bool IsMinimized() const noexcept = 0;
    virtual void Maximize() noexcept = 0;
    virtual void Restore() noexcept = 0;
    virtual bool IsMaximized() const noexcept = 0;

    virtual void SetIcon(const std::filesystem::path& iconPath) = 0;

    virtual uint32_t GetWidth() const noexcept = 0;
    virtual uint32_t GetHeight() const noexcept = 0;

    virtual int GetXClientPos() const noexcept = 0;
    virtual int GetYClientPos() const noexcept = 0;

    virtual const Keyboard& GetKeyboard() const noexcept = 0;
    virtual const Mouse& GetMouse() const noexcept = 0;

    SB_CORE static Ref<Window> Create(const WindowProps& props = {});
};

} //namespace Saba
