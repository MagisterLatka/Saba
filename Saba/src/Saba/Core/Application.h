#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Core/LayerStack.h"
#include "Saba/Core/Window.h"
#include "Saba/Renderer/GraphicsContext.h"
#include "Saba/Renderer/RendererAPI.h"
#include "Saba/Events/ApplicationEvents.h"
#include "Saba/ImGui/ImGuiLayer.h"
#include "Saba/Renderer/Texture.h"
#include "Saba/Core/Buffer.h"

int main(int argc, char** argv, char** envp);

namespace Saba {

struct ApplicationSpecifications {
    std::string Name = "Saba Engine";
    uint32_t Width = 1280u, Height = 720u;
    std::filesystem::path WindowIconPath;
    Buffer TitleBarIconData;
    bool ResizableWindow = true, CustomTitleBar = false, DrawUI = true;

    RendererAPI::API GraphicsAPI
#if defined(SB_PLATFORM_WINDOWS)
    = RendererAPI::API::DX11;
#else
    = RendererAPI::API::OpenGL;
#endif
};

class Application
{
    friend int ::main(int, char**, char**);
public:
    using MenuBarCallbackFn = std::function<void()>;

    SB_CORE Application(ApplicationSpecifications applicationSpecifications = ApplicationSpecifications());
    virtual ~Application() = default;

    SB_CORE void Close() noexcept;

    Layer* PushLayer(Layer* layer) { return m_LayerStack->PushLayer(layer); }
    Layer* PushOverlay(Layer* overlay) { return m_LayerStack->PushOverlay(overlay); }

    Ref<GraphicsContext> GetGraphicsContext() const noexcept { return m_GraphicsContext; }
    Ref<Window> GetWindow() const noexcept { return m_Window; }
    ImGuiLayer* GetImGuiLayer() const noexcept { return m_ImGuiLayer; }
    void SetMenuBarCallback(MenuBarCallbackFn callback) noexcept { m_MenuBarCallback = std::move(callback); }

    const ApplicationSpecifications& GetApplicationSpecifications() const noexcept { return m_Specs; }

    static Application& Get() noexcept { return *s_Application; }
private:
    SB_CORE void Init();
    SB_CORE void Shutdown();
    SB_CORE int Run();

    void OnEvent(Event& e);
    bool OnWindowClose(WindowCloseEvent& e) noexcept;
    bool OnWindowResize(WindowResizeEvent& e) noexcept;

    void ImGuiRender();
    void DrawTitleBar(float& titleBarHeight);
    void DrawMenuBarUI();
private:
    ApplicationSpecifications m_Specs;

    Ref<GraphicsContext> m_GraphicsContext;
    Ref<Window> m_Window;

    Scope<LayerStack> m_LayerStack;
    ImGuiLayer* m_ImGuiLayer;
    MenuBarCallbackFn m_MenuBarCallback;
    bool m_TitleBarHovered = false;
    Ref<Texture2D> m_TitleBarIcon, m_CloseIcon, m_MinimizeIcon, m_MaximizeIcon, m_RestoreIcon;

    Timer m_Timer;
    Timestep m_Timestep;

    bool m_Running = true, m_Minimized = false;

    SB_CORE static Application* s_Application;
};

Application* CreateApplication();

} //namespace Saba
