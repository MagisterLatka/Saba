#include <pch.h>
#include <Saba.h>
#include "ExampleLayer.h"

#include <imgui.h>

ExampleLayer::ExampleLayer() {

}
ExampleLayer::~ExampleLayer() {

}

void ExampleLayer::OnAttach() {
    auto window = Saba::Application::Get().GetWindow();
    m_RenderPass = Saba::RenderPass::Create();
    m_RenderPass->SetRenderTarget(0u, Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight()));

    Saba::Texture2DProps textureProps;
    textureProps.Filepath = "assets/textures/checkerboard.png";
    textureProps.Sampling = Saba::TextureSampling::Point;
    m_Texture = Saba::Texture2D::Create(std::move(textureProps));

    float aspectRatio = static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight());
    Saba::Renderer2D::SetViewProjectionMatrix(glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f));

    m_Scene = Ref<Saba::Scene>::Create("App scene");

    const int count = 150;
    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < count; ++j) {
            auto entity = m_Scene->CreateEntity();
            entity.GetTransformComponent().Position = glm::vec3(-0.5f + static_cast<float>(j) * (1.0f / count), -0.5f + static_cast<float>(i) * (1.0f / count), -0.1f);
            entity.GetTransformComponent().Size = { (0.5f / count), (0.5f / count), 1.0f };
            entity.AddComponent<Saba::SpriteComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }
    }

    m_Quad = m_Scene->CreateEntity("Textured quad");
    m_Quad.AddComponent<Saba::SpriteComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), m_Texture);

}
void ExampleLayer::OnDetach() {

}
void ExampleLayer::OnUpdate([[maybe_unused]] Saba::Timestep ts) {
    Saba::Application::Get().GetWindow()->BindWindow();

    if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f) {
        m_RenderPass->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
        float aspectRatio = m_ViewportSize.x / m_ViewportSize.y;
        Saba::Renderer2D::SetViewProjectionMatrix(glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f));
    }

    m_RenderPass->Bind();
    m_RenderPass->Clear();
    Saba::Renderer2D::ResetStats();

    static float time = 0.0f;
    time += static_cast<float>(ts);
    time = glm::mod(time, glm::two_pi<float>());
    m_Quad.GetTransformComponent().Orientation.z = time;
    m_Scene->OnUpdate(ts);

    Saba::Application::Get().GetWindow()->BindToRender();
    Saba::Application::Get().GetWindow()->Clear();
}
void ExampleLayer::OnUIRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Settings");
    ImGui::Text("Frame time: %.3fms (%.1f fps)", static_cast<double>(1000.0f / io.Framerate), static_cast<double>(io.Framerate));
    ImGui::Text("Draw calls: %d", Saba::Renderer2D::GetStats().DrawCalls);
    ImGui::Text("QuadCount: %d", Saba::Renderer2D::GetStats().QuadCount);
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
        ImGui::Image((uint64_t)m_RenderPass->GetRenderTarget()->GetRawTexturePointer(), viewportSize, { 0, 1 }, { 1, 0 });
    else
        ImGui::Image((uint64_t)m_RenderPass->GetRenderTarget()->GetRawTexturePointer(), viewportSize);

    ImGui::End();
    ImGui::PopStyleVar();
}
