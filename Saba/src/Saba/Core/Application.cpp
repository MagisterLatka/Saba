#include <pch.h>
#include "Application.h"

#include "Saba/Renderer/Renderer.h"
#include "Saba/ImGui/ImGui.h"

#include "Saba/Embed/DefaultIcon.embed"
#include "Saba/Embed/WindowIcons.embed"

namespace Saba {

SB_CORE Application* Application::s_Application = nullptr;
SB_CORE RendererAPI::API RendererAPI::s_API;

Application::Application(ApplicationSpecifications applicationSpecifications)
    : m_Specs(std::move(applicationSpecifications)) {
    SB_CORE_ASSERT(!s_Application, "Application already exist!");
    s_Application = this;

    m_LayerStack = CreateScope<LayerStack>();
}

void Application::Init() {
    //create graphics context
    RendererAPI::s_API = m_Specs.GraphicsAPI;
    m_GraphicsContext = GraphicsContext::Create();
    m_GraphicsContext->Init();

    WindowProps windowProps;
    windowProps.Width = m_Specs.Width;
    windowProps.Height = m_Specs.Height;
    windowProps.Title = m_Specs.Name;
    windowProps.Resizable = m_Specs.ResizableWindow;
    windowProps.HasTitleBar = !m_Specs.CustomTitleBar;
    m_Window = Window::Create(windowProps);
    m_Window->SetEventCallback(SB_BIND_EVENT_FN(Application::OnEvent));
    m_Window->SetTitleBarHitTestCallback([this]([[maybe_unused]] const int xMousePos, [[maybe_unused]] const int yMousePos, int& hitTest) {
        hitTest = m_TitleBarHovered;
    });
    if (!m_Specs.WindowIconPath.empty())
        m_Window->SetIcon(m_Specs.WindowIconPath);

    //init renderer command queue & shader library
    Renderer::Init();

    //init ui layer
    m_ImGuiLayer = new ImGuiLayer;
    m_LayerStack->PushOverlay(m_ImGuiLayer);

    //call OnAttach() on layers in LayerStack
    m_LayerStack->Init();

    uint32_t width, height;
    void* data = nullptr;
    if (m_Specs.TitleBarIconData)
        data = Texture2D::Decode(m_Specs.TitleBarIconData.Data, m_Specs.TitleBarIconData.Size, width, height);
    else
        data = Texture2D::Decode(reinterpret_cast<const void*>(s_DefaultIconData), sizeof(s_DefaultIconData), width, height);
    m_TitleBarIcon = Texture2D::Create(width, height, data);

    data = Texture2D::Decode(reinterpret_cast<const void*>(s_WindowCloseIcon), sizeof(s_WindowCloseIcon), width, height);
    m_CloseIcon = Texture2D::Create(width, height, data);

    data = Texture2D::Decode(reinterpret_cast<const void*>(s_WindowMaximizeIcon), sizeof(s_WindowMaximizeIcon), width, height);
    m_MaximizeIcon = Texture2D::Create(width, height, data);

    data = Texture2D::Decode(reinterpret_cast<const void*>(s_WindowMinimizeIcon), sizeof(s_WindowMinimizeIcon), width, height);
    m_MinimizeIcon = Texture2D::Create(width, height, data);

    data = Texture2D::Decode(reinterpret_cast<const void*>(s_WindowRestoreIcon), sizeof(s_WindowRestoreIcon), width, height);
    m_RestoreIcon = Texture2D::Create(width, height, data);

    Renderer::Render();
}
void Application::Shutdown() {
    m_LayerStack.reset();

    m_TitleBarIcon.Reset();
    m_CloseIcon.Reset();
    m_MaximizeIcon.Reset();
    m_MinimizeIcon.Reset();
    m_RestoreIcon.Reset();

    Renderer::Shutdown();
    m_Window.Reset();
    m_GraphicsContext.Reset();
}
void Application::Close() noexcept {
    m_Running = false;
}

int Application::Run() {
    m_Timer.Reset();
    int returnVal = 0; //return val from quit message
    while (m_Running) {
        //calc time since last frame
        m_Timestep = m_Timer.Elapsed();
        m_Timer.Reset();
        { auto e = AppTickEvent(m_Timestep); OnEvent(e); }

        //update layers
        if (!m_Minimized) {
            for (auto* layer : *m_LayerStack)
                layer->OnUpdate(m_Timestep);
        }
        { auto e = AppUpdateEvent(); OnEvent(e); }

        Renderer::Render();

        //update ui
        m_ImGuiLayer->Begin();
        ImGuiRender();
        m_ImGuiLayer->End();

        //update frame
        m_Window->OnUpdate();
        { auto e = AppRenderEvent(); OnEvent(e); }

        //process events
        if (auto a = Window::ProcessEvents(); a.has_value()) {
            returnVal = *a;
            m_Running = false;
        }
    }
    return returnVal;
}

void Application::OnEvent(Event& e) {
    //dispatch events for application::events
    Dispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(SB_BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(SB_BIND_EVENT_FN(Application::OnWindowResize));

    //pass events to layers
    if (m_LayerStack) {
        for (auto* layer : *m_LayerStack | std::views::reverse) {
            layer->OnEvent(e);
            if (e.p_Handled)
                break;
        }
    }
}
bool Application::OnWindowClose([[maybe_unused]] WindowCloseEvent& e) noexcept {
    m_Running = false;
    return false;
}
bool Application::OnWindowResize(WindowResizeEvent& e) noexcept {
    m_Minimized = e.GetXSize() == 0 || e.GetYSize() == 0;
    return false;
}

void Application::ImGuiRender() {
    if (!m_Specs.DrawUI) {
        for (auto *layer : *m_LayerStack)
        layer->OnUIRender();
        return;
    }

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (!m_Specs.CustomTitleBar && m_MenuBarCallback)
        windowFlags |= ImGuiWindowFlags_MenuBar;

    const bool isMaximized = m_Window->IsMaximized();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });

    ImGui::Begin("DockSpaceWindow", nullptr, windowFlags);

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(4);

    if (!m_Window->IsMaximized()) {
        ImGui::PushStyleColor(ImGuiCol_Border, 0xff303030u);
        UI::RenderWindowOuterBorders(ImGui::GetCurrentWindow());
        ImGui::PopStyleColor();
    }

    if (m_Specs.CustomTitleBar) {
        float titlebarHeight;
        DrawTitleBar(titlebarHeight);
        ImGui::SetCursorPosY(titlebarHeight);
    }

    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    ImGui::DockSpace(ImGui::GetID("MyDockspace"));
    style.WindowMinSize.x = minWinSizeX;

    if (!m_Specs.CustomTitleBar)
        DrawMenuBarUI();

    for (auto *layer : *m_LayerStack)
        layer->OnUIRender();

    ImGui::End();
}
void Application::DrawTitleBar(float& outTitlebarHeight)
{
    constexpr float titlebarHeight = 58.0f;
    const bool isMaximized = m_Window->IsMaximized();
    const float titlebarVerticalOffset = isMaximized ? -6.0f : 0.0f;
    const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;
    //constexpr ImU32 debugColor = 0xff2020ffu;

    ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
    const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
    const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
                                 ImGui::GetCursorScreenPos().y + titlebarHeight };
    auto* backgroundDrawList = ImGui::GetBackgroundDrawList();
    auto* foregroundDrawList = ImGui::GetForegroundDrawList();
    backgroundDrawList->AddRectFilled(titlebarMin, titlebarMax, 0xff181818u);
    //debug title bar bounds
    // foregroundDrawList->AddRect(titlebarMin, titlebarMax, debugColor);

    uint32_t iconWidth = 48;
    uint32_t iconHeight = 48;
    const ImVec2 iconOffset(16.0f + windowPadding.x, 5.0f + windowPadding.y + titlebarVerticalOffset);
    const ImVec2 iconRectMin = { ImGui::GetItemRectMin().x + iconOffset.x, ImGui::GetItemRectMin().y + iconOffset.y };
    const ImVec2 iconRectMax = { iconRectMin.x + static_cast<float>(iconWidth), iconRectMin.y + static_cast<float>(iconHeight) };
    foregroundDrawList->AddImage(m_TitleBarIcon->GetRawPointer(), iconRectMin, iconRectMax);

    ImGui::BeginHorizontal("Titlebar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing() });

    const float width = ImGui::GetContentRegionAvail().x;
    const float buttonsAreaWidth = 112.0f;

    ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
    //debug drag bounds
    // foregroundDrawList->AddRect(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + width - buttonsAreaWidth,
    //            ImGui::GetCursorScreenPos().y + titlebarHeight), debugColor);
    ImGui::SetNextItemAllowOverlap();
    ImGui::InvisibleButton("##titleBarDragZone", ImVec2(width - buttonsAreaWidth, titlebarHeight));

    m_TitleBarHovered = ImGui::IsItemHovered();

    if (isMaximized) {
        float windowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
        if (windowMousePosY >= 0.0f && windowMousePosY <= 5.0f)
            m_TitleBarHovered = true;
    }

    if (m_MenuBarCallback) {
        ImGui::SuspendLayout();

        const float iconHorizontalOffset = 16.0f * 2.0f + static_cast<float>(iconWidth) + windowPadding.x;
        ImGui::SetCursorPos(ImVec2(iconHorizontalOffset, 6.0f + titlebarVerticalOffset));
        DrawMenuBarUI();

        if (ImGui::IsItemHovered())
            m_TitleBarHovered = false;

        ImGui::ResumeLayout();
    }

    const ImVec2 currentCursorPos = ImGui::GetCursorPos();
    const ImVec2 textSize = ImGui::CalcTextSize(m_Specs.Name.c_str());
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
    ImGui::Text("%s", m_Specs.Name.c_str());
    ImGui::SetCursorPos(currentCursorPos);

    const ImU32 buttonNormalColor = 0xffb0b0b0u;
    const ImU32 buttonHoveredColor = 0xffd0d0d0u;
    const ImU32 buttonPressedColor = 0xffd8d8d8u;
    const float buttonWidth = 14.0f;
    const float buttonHeight = 14.0f;

    ImGui::Spring();
    UI::ShiftCursorX(8.0f);
    {
        iconWidth = m_MinimizeIcon->GetWidth();
        iconHeight = m_MinimizeIcon->GetHeight();
        const float pad = (buttonHeight - static_cast<float>(iconHeight)) / 2.0f;
        if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight)))
            m_Window->Minimize();

        UI::DrawButtonImage(m_MinimizeIcon, buttonNormalColor, buttonHoveredColor, buttonPressedColor, UI::RectExpand(UI::GetItemRect(), 0.0f, -pad));
    }

    ImGui::Spring(-1.0f, 17.0f);
    UI::ShiftCursorX(8.0f);
    {
        iconWidth = m_MaximizeIcon->GetWidth();
        iconHeight = m_MaximizeIcon->GetHeight();
        const bool maximized = m_Window->IsMaximized();

        if (ImGui::InvisibleButton("Maximize", ImVec2(buttonWidth, buttonHeight))) {
            if (maximized)
                m_Window->Restore();
            else
                m_Window->Maximize();
        }

        UI::DrawButtonImage(maximized ? m_RestoreIcon : m_MaximizeIcon, buttonNormalColor, buttonHoveredColor, buttonPressedColor, UI::GetItemRect());
    }

    ImGui::Spring(-1.0f, 15.0f);
    UI::ShiftCursorX(8.0f);
    {
        iconWidth = m_CloseIcon->GetWidth();
        iconHeight = m_CloseIcon->GetHeight();

        if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight)))
            m_Running = false;

        UI::DrawButtonImage(m_CloseIcon, buttonNormalColor, buttonHoveredColor, buttonPressedColor, UI::GetItemRect());
    }

    ImGui::Spring(-1.0f, 18.0f);
    ImGui::EndHorizontal();
    outTitlebarHeight = titlebarHeight;
}
void Application::DrawMenuBarUI() {
    if (!m_MenuBarCallback)
        return;

    if (m_Specs.CustomTitleBar) {
        const ImRect menuBarRect = { ImGui::GetCursorPos(), { ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing() } };

        ImGui::BeginGroup();
        if (UI::BeginMenuBar(menuBarRect)) {
            m_MenuBarCallback();
            UI::EndMenuBar();
        }
        ImGui::EndGroup();
    }
    else if (ImGui::BeginMenuBar()) {
        m_MenuBarCallback();
        ImGui::EndMenuBar();
    }
}

} //namespace Saba
