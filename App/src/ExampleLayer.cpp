#include <pch.h>
#include <Saba.h>
#include "ExampleLayer.h"

#include <imgui.h>

ExampleLayer::ExampleLayer() {

}
ExampleLayer::~ExampleLayer() {

}

void ExampleLayer::OnAttach() {
    if (Saba::RendererAPI::GetAPI() == Saba::RendererAPI::API::DX11)
        m_Shader = Saba::Shader::Create(L"assets/shaders/basic.vert.cso", L"assets/shaders/basic.frag.cso");
    else
        m_Shader = Saba::Shader::Create(L"assets/shaders/basic.shader");

    float vertices[] = {
         0.0f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f
    };
    Saba::BufferLayout layout = {
        { "Position", Saba::BufferLayoutElementDataType::Float2 }
    };
    Ref<Saba::VertexBuffer> vbo = Saba::VertexBuffer::Create(layout, vertices, sizeof(vertices));
    m_InputLayout = Saba::InputLayout::Create({ vbo }, m_Shader);

    auto window = Saba::Application::Get().GetWindow();
    m_RenderTarget = Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight());
}
void ExampleLayer::OnDetach() {

}
void ExampleLayer::OnUpdate([[maybe_unused]] Saba::Timestep ts) {
    Saba::Application::Get().GetWindow()->BindWindow();

    if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
        m_RenderTarget->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));

    m_RenderTarget->Bind();
    m_RenderTarget->Clear();

    m_Shader->Bind();
    m_InputLayout->Bind();

    Saba::RenderCommand::Draw(Saba::RendererAPI::Topology::Triangles, 3u);

    Saba::Application::Get().GetWindow()->BindToRender();
    Saba::Application::Get().GetWindow()->Clear();
    Saba::Renderer::Render();
}
void ExampleLayer::OnUIRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Settings");
    ImGui::Text("Frame time: %.3fms (%.1f fps)", static_cast<double>(1000.0f / io.Framerate), static_cast<double>(io.Framerate));
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    auto& application = Saba::Application::Get();

    m_ViewportHovered = ImGui::IsWindowHovered();
    m_ViewportFocused = ImGui::IsWindowFocused();
    application.GetImGuiLayer()->BlockEvents(!m_ViewportHovered && !m_ViewportFocused);

    auto viewportSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportSize.x, viewportSize.y };
    ImVec2 viewportPos = ImGui::GetWindowPos();
    ImVec2 offset = ImGui::GetWindowContentRegionMin();
    viewportPos.x += offset.x;
    viewportPos.y += offset.y;
    m_ViewportPos = { viewportPos.x - static_cast<float>(application.GetWindow()->GetXClientPos()),
        viewportPos.y - static_cast<float>(application.GetWindow()->GetYClientPos()) };

    if (Saba::RendererAPI::GetAPI() == Saba::RendererAPI::API::OpenGL)
        ImGui::Image((uint64_t)m_RenderTarget->GetRawTexturePointer(), viewportSize, { 0, 1 }, { 1, 0 });
    else
        ImGui::Image((uint64_t)m_RenderTarget->GetRawTexturePointer(), viewportSize);

    ImGui::End();
    ImGui::PopStyleVar();
}
