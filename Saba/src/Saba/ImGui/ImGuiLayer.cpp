#include <pch.h>
#include "ImGuiLayer.h"

#if defined(SB_PLATFORM_WINDOWS)
#    include <backends/imgui_impl_win32.h>
#    include <backends/imgui_impl_dx11.h>
//#    include <backends/imgui_impl_dx12.h>
#    include <Saba/Platform/Windows/WindowsWindow.h>
#    include <Saba/Platform/DX11/DX11Context.h>
#else
#    include <backends/imgui_impl_glfw.h>
#    include <Saba/Platform/Linux/LinuxWindow.h>
#endif

#include <backends/imgui_impl_opengl3.h>
//#include <backends/imgui_impl_vulkan.h>

#include "Saba/ImGui/ImGuizmo.h"

#include "Saba/Core/Application.h"

#include "Saba/Embed/Font.embed"

namespace Saba {

static void SetDarkThemeColors() {
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}
void ImGuiLayer::OnAttach() {
    IMGUI_CHECKVERSION();
    m_Context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;
    io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(const_cast<uint8_t*>(s_FontData)), sizeof(s_FontData), 18.0f, &config);
    io.FontDefault = io.Fonts->Fonts.back();

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 2.5f;
    style.FrameBorderSize = 1.0f;
    style.IndentSpacing = 11.0f;
    style.WindowPadding = ImVec2(10.0f, 10.0f);
    style.FramePadding = ImVec2(8.0f, 6.0f);
    style.ItemSpacing = ImVec2(6.0f, 6.0f);
    style.ChildRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.FrameRounding = 6.0f;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    SetDarkThemeColors();

    Init();
}
void ImGuiLayer::Init() {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported");  break;
        case RendererAPI::API::OpenGL: {
#if defined(SB_PLATFORM_WINDOWS)
            auto window = Application::Get().GetWindow().As<WindowsWindow>();
            ImGui_ImplWin32_InitForOpenGL(window->m_Window);
            window->HandleImGuiInput();
#else
            auto window = Application::Get().GetWindow().As<LinuxWindow>();
            ImGui_ImplGlfw_InitForOpenGL(window->m_Window, true);
#endif
            ImGui_ImplOpenGL3_Init("#version 460");
            break;
        }
        case RendererAPI::API::Vulkan:    SB_CORE_THROW_INFO("Vulkan is not supported yet"); break;
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11: {
            auto window = Application::Get().GetWindow().As<WindowsWindow>();
            auto context = DX11Context::GetContextFromApplication();
            ImGui_ImplWin32_Init(window->m_Window);
            window->HandleImGuiInput();
            ImGui_ImplDX11_Init(context->GetDevice().Get(), context->GetContext().Get());
            break;
        }
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); break;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); break;
#endif
    }
}
void ImGuiLayer::OnDetach() {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported");  break;
        case RendererAPI::API::OpenGL: {
            ImGui_ImplOpenGL3_Shutdown();
#if defined(SB_PLATFORM_WINDOWS)
            ImGui_ImplWin32_Shutdown();
#else
            ImGui_ImplGlfw_Shutdown();
#endif
            break;
        }
        case RendererAPI::API::Vulkan:    SB_CORE_THROW_INFO("Vulkan is not supported yet"); break;
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11: {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            break;
        }
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); break;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); break;
#endif
    }
    ImGui::DestroyContext();
}

void ImGuiLayer::OnEvent(Event& e) {
    ImGuiIO& io = ImGui::GetIO();
    if (m_BlockEvents) {
        e.p_Handled |= static_cast<int>(e.IsInCategory(EventCategoryMouse)) & static_cast<int>(io.WantCaptureMouse);
        e.p_Handled |= static_cast<int>(e.IsInCategory(EventCategoryKeyboard)) & static_cast<int>(io.WantCaptureKeyboard);
    }
}

void ImGuiLayer::Begin() {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported");  break;
        case RendererAPI::API::OpenGL: {
#if defined(SB_PLATFORM_WINDOWS)
            ImGui_ImplWin32_NewFrame();
#else
            ImGui_ImplGlfw_NewFrame();
#endif
            ImGui_ImplOpenGL3_NewFrame();
            break;
        }
        case RendererAPI::API::Vulkan:    SB_CORE_THROW_INFO("Vulkan is not supported yet"); break;
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11: {
            ImGui_ImplWin32_NewFrame();
            ImGui_ImplDX11_NewFrame();
            break;
        }
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); break;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); break;
#endif
    }
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}
void ImGuiLayer::End() {
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow()->GetWidth()), static_cast<float>(app.GetWindow()->GetHeight()));

    ImGui::Render();

    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported");  break;
        case RendererAPI::API::OpenGL: {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            break;
        }
        case RendererAPI::API::Vulkan:    SB_CORE_THROW_INFO("Vulkan is not supported yet"); break;
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11: {
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            break;
        }
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); break;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); break;
#endif
    }

    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
#if defined(SB_PLATFORM_WINDOWS)
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
#else
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        if (backup_current_context != glfwGetCurrentContext())
            glfwMakeContextCurrent(backup_current_context);
#endif
    }
}

}
