#include <pch.h>
#include <Saba.h>
#include "ExampleLayer.h"

#include <Saba/ImGui/ImGui.h>
#include <Saba/ImGui/ImGuizmo.h>

void ExampleLayer::OnAttach() {
    auto window = Saba::Application::Get().GetWindow();
    m_RenderPass = Saba::RenderPass::Create();
    m_RenderPass->SetRenderTarget(0u, Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight()));
    m_RenderPass->SetRenderTarget(1u, Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight(), Saba::RenderTargetFormat::R32_UINT));
    m_RenderPass->SetDepthStencilTarget(Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight(), Saba::RenderTargetFormat::Depth32F));
    Saba::RenderCommand::SetRasterizerOptions(Saba::RendererAPI::TriangleFillMode::Full, Saba::RendererAPI::TriangleCullMode::DrawAll);
    // Saba::RenderCommand::SetBlendOptions(0u, true, Saba::RendererAPI::BlendOption::SourceAlpha, Saba::RendererAPI::BlendOption::SourceAlphaInvert,
    //     Saba::RendererAPI::BlendOperation::Add, Saba::RendererAPI::BlendOption::SourceAlpha, Saba::RendererAPI::BlendOption::SourceAlphaInvert);
    // Saba::RenderCommand::SetBlendOptions(1u, false);

    Saba::Texture2DProps textureProps;
    textureProps.Sampling = Saba::TextureSampling::Point;

    m_Scene = Ref<Saba::Scene>::Create();
    m_Scene->OnViewportResize(window->GetWidth(), window->GetHeight());

    textureProps.Filepath = "assets/textures/brickwall.jpg";
    auto brickTexture = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/brickwall_normal.jpg";
    auto brickTextureNormal = Saba::Texture2D::Create(textureProps); 
    auto brickMaterial = Ref<Saba::Material>::Create(brickTexture, brickTextureNormal);
    auto brickModel = Ref<Saba::Model>::Create(std::initializer_list{ Saba::Renderer::GetMeshLibrary().Get("Cube") }, brickMaterial);
    auto brickCube = m_Scene->CreateEntity("Brick cube");
    brickCube.AddComponent<Saba::ModelComponent>(std::move(brickModel));
    brickCube.GetComponent<Saba::TransformComponent>().Position = glm::vec3(0.0f, 0.0f, -1.0f);

    textureProps.Filepath = "assets/textures/RustedIron/rustediron2_basecolor.png";
    auto rustedTexture = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/RustedIron/rustediron2_normal.png";
    auto rustedTextureNormal = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/RustedIron/rustediron2_metallic.png";
    auto rustedTextureMetallic = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/RustedIron/rustediron2_roughness.png";
    auto rustedTextureRoughness = Saba::Texture2D::Create(textureProps);
    auto rustedMaterial = Ref<Saba::Material>::Create(rustedTexture, rustedTextureNormal, rustedTextureMetallic, rustedTextureRoughness);
    auto rustedModel = Ref<Saba::Model>::Create(std::initializer_list{ Saba::Renderer::GetMeshLibrary().Get("Cube") }, rustedMaterial);
    auto rustedCube = m_Scene->CreateEntity("Rusted cube");
    rustedCube.AddComponent<Saba::ModelComponent>(rustedModel);
    rustedCube.GetComponent<Saba::TransformComponent>().Position = glm::vec3(-1.0f, 0.0f, 1.0f);

    textureProps.Filepath = "assets/textures/ClayShingles/clay-shingles1_albedo.png";
    auto clayTexture = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/ClayShingles/clay-shingles1_normal-ogl.png";
    auto clayTextureNormal = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/ClayShingles/clay-shingles1_metallic.png";
    auto clayTextureMetallic = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/ClayShingles/clay-shingles1_roughness.png";
    auto clayTextureRoughness = Saba::Texture2D::Create(textureProps);
    auto clayMaterial = Ref<Saba::Material>::Create(clayTexture, clayTextureNormal, clayTextureMetallic, clayTextureRoughness);
    auto clayModel = Ref<Saba::Model>::Create(std::initializer_list{ Saba::Renderer::GetMeshLibrary().Get("Cube") }, clayMaterial);
    auto clayCube = m_Scene->CreateEntity("Clay cube");
    clayCube.AddComponent<Saba::ModelComponent>(clayModel);
    clayCube.GetComponent<Saba::TransformComponent>().Position = glm::vec3(1.0f, 0.0f, 2.0f);

    auto sphere = m_Scene->CreateEntity("Sphere");
    sphere.GetTransformComponent().Position = glm::vec3(0.0f, -2.0f, 0.0f);
    sphere.GetTransformComponent().Size = glm::vec3(0.5f, 0.5f, 0.5f);
    auto model = Ref<Saba::Model>::Create("assets/models/sphere.fbx", clayMaterial);
    sphere.AddComponent<Saba::ModelComponent>(model);

    m_Scene->CreateEntity("Light 0").AddComponent<Saba::LightComponent>(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 5.0f), 50.0f);
    m_Scene->CreateEntity("Light 1").AddComponent<Saba::LightComponent>(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 5.0f), 50.0f);

    m_Scene->CreateAndSetCameraEntity(Ref<Saba::PerspectiveCamera>::Create(16.0f / 9.0f)).GetTransformComponent().Position.z = 3.0f;
}
void ExampleLayer::OnDetach() {
    m_Scene.Reset();
    m_RenderPass.Reset();
}
void ExampleLayer::OnUpdate(Saba::Timestep ts) {
    Saba::Application::Get().GetWindow()->BindWindow();

    if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0 && (m_ViewportSize.x != m_RenderPass->GetWidth() || m_ViewportSize.y != m_RenderPass->GetHeight())) {
        m_RenderPass->Resize(m_ViewportSize.x, m_ViewportSize.y);
        m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    m_RenderPass->Bind();
    m_RenderPass->Clear();
    Saba::RenderCommand::SetDepthTestOptions(true);

    m_Scene->OnUpdateRuntime(ts);
    Saba::Renderer::Render();

    Saba::Application::Get().GetWindow()->BindToRender();
    Saba::Application::Get().GetWindow()->Clear();

    Saba::RenderCommand::SetDepthTestOptions(false);
    m_RenderPass->GetRenderTarget()->BindTexture();
    Saba::Renderer2D::DrawFullscreenQuad();
}
void ExampleLayer::OnUIRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Settings");
    ImGui::Text("Frame time: %.3fms (%.1f fps)", static_cast<double>(1000.0f / io.Framerate), static_cast<double>(io.Framerate));
    ImGui::End();
}
