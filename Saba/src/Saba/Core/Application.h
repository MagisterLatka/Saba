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

struct ApplicationSpecifications
{
    std::string Name = "Saba Engine";
    uint32_t Width = 1280u, Height = 900u;
    std::filesystem::path WindowIconPath;
    Buffer TitleBarIconData;
    bool ResizableWindow = true, CustomTitleBar = false;

#if defined(SB_PLATFORM_WINDOWS)
    RendererAPI::API GraphicsAPI = RendererAPI::API::DX11;
#else
    RendererAPI::API GraphicsAPI = RendererAPI::API::OpenGL;
#endif
};

class Application
{
    friend int ::main(int, char**, char**);
public:
    using MenuBarCallbackFn = std::function<void()>;

    SB_CORE Application(ApplicationSpecifications applicationSpecifications = ApplicationSpecifications());
    SB_CORE virtual ~Application() = default;

    SB_CORE void Close() noexcept;

    SB_CORE Layer* PushLayer(Layer* layer) { return m_LayerStack->PushLayer(layer); }
    SB_CORE Layer* PushOverlay(Layer* overlay) { return m_LayerStack->PushOverlay(overlay); }

    SB_CORE Ref<GraphicsContext> GetGraphicsContext() const noexcept { return m_GraphicsContext; }
    SB_CORE Ref<Window> GetWindow() const noexcept { return m_Window; }
    SB_CORE ImGuiLayer* GetImGuiLayer() const noexcept { return m_ImGuiLayer; }
    SB_CORE void SetMenuBarCallback(MenuBarCallbackFn callback) noexcept { m_MenuBarCallback = std::move(callback); }

    SB_CORE const ApplicationSpecifications& GetApplicationSpecifications() const noexcept { return m_Specs; }

    SB_CORE static Application& Get() noexcept { return *s_Application; }
private:
    SB_CORE void Init();
    SB_CORE void Shutdown();
    SB_CORE int Run();

    SB_CORE void OnEvent(Event& e);
    SB_CORE bool OnWindowClose(WindowCloseEvent& e) noexcept;
    SB_CORE bool OnWindowResize(WindowResizeEvent& e) noexcept;

    SB_CORE void ImGuiRender();
    SB_CORE void DrawTitleBar(float& titleBarHeight);
    SB_CORE void DrawMenuBarUI();
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
