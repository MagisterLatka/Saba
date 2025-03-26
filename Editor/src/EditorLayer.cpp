#include <pch.h>
#include <Saba.h>
#include "EditorLayer.h"

#include <Saba/ImGui/ImGui.h>
#include <Saba/ImGui/ImGuizmo.h>

void EditorLayer::OnAttach() {
    auto window = Saba::Application::Get().GetWindow();
    m_RenderPass = Saba::RenderPass::Create();
    m_RenderPass->SetRenderTarget(0u, Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight()));
    m_RenderPass->SetRenderTarget(1u, Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight(), Saba::RenderTargetFormat::R32_UINT));
    m_RenderPass->SetDepthStencilTarget(Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight(), Saba::RenderTargetFormat::Depth32F));
    Saba::RenderCommand::SetDepthTestOptions(true);
    Saba::RenderCommand::SetRasterizerOptions(Saba::RendererAPI::TriangleFillMode::Full, Saba::RendererAPI::TriangleCullMode::DrawAll);
    // Saba::RenderCommand::SetBlendOptions(0u, true, Saba::RendererAPI::BlendOption::SourceAlpha, Saba::RendererAPI::BlendOption::SourceAlphaInvert,
    //     Saba::RendererAPI::BlendOperation::Add, Saba::RendererAPI::BlendOption::SourceAlpha, Saba::RendererAPI::BlendOption::SourceAlphaInvert);
    // Saba::RenderCommand::SetBlendOptions(1u, false);

    Saba::Texture2DProps textureProps;
    textureProps.Filepath = "assets/textures/PlayButton.png";
    textureProps.Sampling = Saba::TextureSampling::Point;
    m_PlayButton = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/StopButton.png";
    m_StopButton = Saba::Texture2D::Create(textureProps);

    m_ActiveScene = Ref<Saba::Scene>::Create();
    m_ActiveScene->OnViewportResize(window->GetWidth(), window->GetHeight());
    m_EditorScene = m_ActiveScene;

    // std::vector<Saba::MeshVertex> vertices = {
    //     { { -0.5f, -0.5f, -0.5f, 1.0f }, {  0.0f, -1.0f,  0.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f} },
    //     { {  0.5f, -0.5f, -0.5f, 1.0f }, {  0.0f, -1.0f,  0.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f} },
    //     { {  0.5f, -0.5f,  0.5f, 1.0f }, {  0.0f, -1.0f,  0.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f} },
    //     { { -0.5f, -0.5f,  0.5f, 1.0f }, {  0.0f, -1.0f,  0.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f} },

    //     { { -0.5f,  0.5f, -0.5f, 1.0f }, {  0.0f,  1.0f,  0.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f} },
    //     { {  0.5f,  0.5f, -0.5f, 1.0f }, {  0.0f,  1.0f,  0.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f} },
    //     { {  0.5f,  0.5f,  0.5f, 1.0f }, {  0.0f,  1.0f,  0.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f} },
    //     { { -0.5f,  0.5f,  0.5f, 1.0f }, {  0.0f,  1.0f,  0.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f} },

    //     { { -0.5f, -0.5f, -0.5f, 1.0f }, { -1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f} },
    //     { { -0.5f,  0.5f, -0.5f, 1.0f }, { -1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f} },
    //     { { -0.5f,  0.5f,  0.5f, 1.0f }, { -1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f} },
    //     { { -0.5f, -0.5f,  0.5f, 1.0f }, { -1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f} },

    //     { {  0.5f, -0.5f, -0.5f, 1.0f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f} },
    //     { {  0.5f,  0.5f, -0.5f, 1.0f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f} },
    //     { {  0.5f,  0.5f,  0.5f, 1.0f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f} },
    //     { {  0.5f, -0.5f,  0.5f, 1.0f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f} },

    //     { { -0.5f, -0.5f, -0.5f, 1.0f }, {  0.0f,  0.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f} },
    //     { {  0.5f, -0.5f, -0.5f, 1.0f }, {  0.0f,  0.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f} },
    //     { {  0.5f,  0.5f, -0.5f, 1.0f }, {  0.0f,  0.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f} },
    //     { { -0.5f,  0.5f, -0.5f, 1.0f }, {  0.0f,  0.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f} },

    //     { { -0.5f, -0.5f,  0.5f, 1.0f }, {  0.0f,  0.0f,  1.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f} },
    //     { {  0.5f, -0.5f,  0.5f, 1.0f }, {  0.0f,  0.0f,  1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f} },
    //     { {  0.5f,  0.5f,  0.5f, 1.0f }, {  0.0f,  0.0f,  1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f} },
    //     { { -0.5f,  0.5f,  0.5f, 1.0f }, {  0.0f,  0.0f,  1.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f} }
    // };
    // std::vector<uint32_t> indices = {
    //     0, 1, 2, 2, 3, 0,
    //     4, 5, 6, 6, 7, 4,
    //     8, 9, 10, 10, 11, 8,
    //     12, 13, 14, 14, 15, 12,
    //     16, 17, 18, 18, 19, 16,
    //     20, 21, 22, 22, 23, 20
    // };
    // Ref<Saba::Mesh> mesh = Ref<Saba::Mesh>::Create(std::move(vertices), std::move(indices));
    textureProps.Filepath = "assets/textures/brickwall.jpg";
    auto brickTexture = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/brickwall_normal.jpg";
    auto brickTextureNormal = Saba::Texture2D::Create(textureProps); 
    Ref<Saba::Material> brickMaterial = Ref<Saba::Material>::Create(brickTexture, brickTextureNormal);
    auto brickCube = m_ActiveScene->CreateEntity("Brick cube");
    brickCube.AddComponent<Saba::MeshComponent>(Saba::Renderer::GetMeshLibrary().Get("Cube"), brickMaterial);
    brickCube.GetComponent<Saba::TransformComponent>().Position = glm::vec3(0.0f, 0.0f, -1.0f);

    textureProps.Filepath = "assets/textures/RustedIron/rustediron2_basecolor.png";
    auto rustedTexture = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/RustedIron/rustediron2_normal.png";
    auto rustedTextureNormal = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/RustedIron/rustediron2_metallic.png";
    auto rustedTextureMetallic = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/RustedIron/rustediron2_roughness.png";
    auto rustedTextureRoughness = Saba::Texture2D::Create(textureProps);
    Ref<Saba::Material> rustedMaterial = Ref<Saba::Material>::Create(rustedTexture, rustedTextureNormal, rustedTextureMetallic, rustedTextureRoughness);
    auto rustedCube = m_ActiveScene->CreateEntity("Rusted cube");
    rustedCube.AddComponent<Saba::MeshComponent>(Saba::Renderer::GetMeshLibrary().Get("Cube"), rustedMaterial);
    rustedCube.GetComponent<Saba::TransformComponent>().Position = glm::vec3(-1.0f, 0.0f, 1.0f);

    textureProps.Filepath = "assets/textures/ClayShingles/clay-shingles1_albedo.png";
    auto clayTexture = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/ClayShingles/clay-shingles1_normal-ogl.png";
    auto clayTextureNormal = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/ClayShingles/clay-shingles1_metallic.png";
    auto clayTextureMetallic = Saba::Texture2D::Create(textureProps);
    textureProps.Filepath = "assets/textures/ClayShingles/clay-shingles1_roughness.png";
    auto clayTextureRoughness = Saba::Texture2D::Create(textureProps);
    Ref<Saba::Material> clayMaterial = Ref<Saba::Material>::Create(clayTexture, clayTextureNormal, clayTextureMetallic, clayTextureRoughness);
    auto clayCube = m_ActiveScene->CreateEntity("Clay cube");
    clayCube.AddComponent<Saba::MeshComponent>(Saba::Renderer::GetMeshLibrary().Get("Cube"), clayMaterial);
    clayCube.GetComponent<Saba::TransformComponent>().Position = glm::vec3(1.0f, 0.0f, 2.0f);

    m_ActiveScene->CreateEntity("Light 0").AddComponent<Saba::LightComponent>(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 5.0f), 50.0f);
    m_ActiveScene->CreateEntity("Light 1").AddComponent<Saba::LightComponent>(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 5.0f), 50.0f);

    m_SceneHierarchyPanel = CreateScope<Saba::SceneHierarchyPanel>(m_ActiveScene);
    m_ContentBrowserPanel = CreateScope<Saba::ContentBrowserPanel>();

    m_EditorCamera = Saba::EditorCamera(glm::half_pi<float>(), static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()));
}
void EditorLayer::OnDetach() {
    m_ContentBrowserPanel.reset();
    m_SceneHierarchyPanel.reset();
    m_EditorScene.Reset();
    m_ActiveScene.Reset();
    m_RenderPass.Reset();
    m_PlayButton.Reset();
    m_StopButton.Reset();
}
void EditorLayer::OnUpdate([[maybe_unused]] Saba::Timestep ts) {
    Saba::Application::Get().GetWindow()->BindWindow();

    if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0 && (m_ViewportSize.x != m_RenderPass->GetWidth() || m_ViewportSize.y != m_RenderPass->GetHeight())) {
        m_RenderPass->Resize(m_ViewportSize.x, m_ViewportSize.y);
        m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    }

    m_RenderPass->Bind();
    m_RenderPass->Clear();
    Saba::Renderer2D::ResetStats();

    switch (m_SceneState) {
        case SceneState::Edit: {
            m_EditorCamera.OnUpdate();
            m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
            break;
        }
        case SceneState::Play: {
            m_ActiveScene->OnUpdateRuntime(ts);
            break;
        }
    }

    glm::ivec2 mousePos = Saba::Input::GetMousePos();
    mousePos -= m_ViewportPos;
    if (mousePos.x > 0 && mousePos.y >= 0 && mousePos.x < static_cast<int>(m_ViewportSize.x) && mousePos.y < static_cast<int>(m_ViewportSize.y)) {
        static uint32_t data = 0u;
        m_RenderPass->GetRenderTarget(1u)->ReadPixel(&data, static_cast<uint32_t>(sizeof(uint32_t)), static_cast<uint32_t>(mousePos.x), static_cast<uint32_t>(mousePos.y));
        Saba::Renderer::Submit([this]() {
            m_HoveredEntity = (data == 0 || data == std::numeric_limits<uint32_t>::max())
                ? Saba::Entity() : Saba::Entity(static_cast<entt::entity>(data), m_ActiveScene.Raw());
        });
    }

    Saba::Application::Get().GetWindow()->BindToRender();
    Saba::Application::Get().GetWindow()->Clear();
}
void EditorLayer::OnUIRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Settings");
    ImGui::Text("Frame time: %.3fms (%.1f fps)", static_cast<double>(1000.0f / io.Framerate), static_cast<double>(io.Framerate));
    ImGui::Text("Draw calls: %d", Saba::Renderer2D::GetStats().DrawCalls);
    ImGui::Text("Quad count: %d", Saba::Renderer2D::GetStats().QuadCount);
    ImGui::Text("Circle count: %d", Saba::Renderer2D::GetStats().CircleCount);
    std::string name = "None";
    if (m_HoveredEntity)
        name = m_HoveredEntity.GetComponent<Saba::TagComponent>().Tag;
    ImGui::Text("Hovered entity: %s", name.c_str());
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
    m_ViewportPos = { static_cast<int>(viewportPos.x) - application.GetWindow()->GetXClientPos(),
        static_cast<int>(viewportPos.y) - application.GetWindow()->GetYClientPos() };

    Saba::UI::DrawImage(m_RenderPass->GetRenderTarget(), viewportSize);

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            const char* path = reinterpret_cast<const char*>(payload->Data);
            OpenScene(Saba::g_AssetsPath / path);
        }
        ImGui::EndDragDropTarget();
    }

    if (Saba::Entity selected = m_SceneHierarchyPanel->GetSelected(); m_GuizmoType != -1 && selected && m_SceneState == SceneState::Edit) {
        const bool snap = Saba::Input::IsKeyPressed(Saba::KeyCode::LeftControl);
        const float snapValue = m_GuizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : m_GuizmoType == ImGuizmo::OPERATION::SCALE ? 0.5f : 0.1f;
        const float snapValues[3] = { snapValue, snapValue, snapValue };

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        const glm::mat4 viewMat = m_EditorCamera.GetViewMatrix();
        const glm::mat4& projMat = m_EditorCamera.GetProjectionMatrix();
        glm::mat4 transform = selected.HasComponent<Saba::LightComponent>()
            ? glm::translate(selected.GetComponent<Saba::LightComponent>().LightPos)
            : selected.GetTransform();

        ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projMat), static_cast<ImGuizmo::OPERATION>(m_GuizmoType), ImGuizmo::LOCAL,
            glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing()) {
            glm::vec3 pos, size, skew;
            glm::quat orientation;
            glm::vec4 perspective;
            glm::decompose(transform, size, orientation, pos, skew, perspective);
            if (selected.HasComponent<Saba::LightComponent>()) {
                selected.GetComponent<Saba::LightComponent>().LightPos = pos;
            }
            else {
                auto& tc = selected.GetTransformComponent();
                tc.Position = pos;
                tc.Orientation += glm::eulerAngles(orientation) - tc.Orientation;
                tc.Size = size;
            }
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();

    m_SceneHierarchyPanel->OnUIRender();
    m_ContentBrowserPanel->OnUIRender();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const auto& buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    ImGui::Begin("###toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);

    float size = ImGui::GetWindowHeight() - 16.0f;
    Ref<Saba::Texture2D> icon = m_SceneState == SceneState::Edit ? m_PlayButton : m_StopButton;
    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - size * 0.5f);
    if (ImGui::ImageButton("###button", (uint64_t)icon->GetRawPointer(), ImVec2(size, size))) {
        switch (m_SceneState) {
            case EditorLayer::SceneState::Edit:
                OnScenePlay();
                break;
            case EditorLayer::SceneState::Play:
                OnSceneStop();
                break;
        }
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();
}
void EditorLayer::OnEvent(Saba::Event& e) {
    Saba::Dispatcher dispatcher(e);
    dispatcher.Dispatch<Saba::KeyPressedEvent>(SB_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    dispatcher.Dispatch<Saba::MouseButtonPressedEvent>(SB_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));

    if (m_SceneState == SceneState::Play)
        m_ActiveScene->OnEvent(e);
    else
        m_EditorCamera.OnEvent(e);
}
bool EditorLayer::OnKeyPressed(Saba::KeyPressedEvent& e) {
    const bool shift = Saba::Input::IsKeyPressed(Saba::KeyCode::LeftShift);
    const bool control = Saba::Input::IsKeyPressed(Saba::KeyCode::LeftControl);

    switch (e.GetKeyCode()) {
        default:
            break;
        case Saba::KeyCode::N:
            if (control)
                NewScene();
            break;
        case Saba::KeyCode::O:
            if (control)
                OpenScene();
            break;
        case Saba::KeyCode::S:
            if (control) {
                if (shift)
                    SaveSceneAs();
                else
                    SaveScene();
            }
            break;
        case Saba::KeyCode::D:
            if (control)
                OnDuplicateEntity();
            break;
        case Saba::KeyCode::Q:
            if (!ImGuizmo::IsUsing())
                m_GuizmoType = -1;
            break;
        case Saba::KeyCode::W:
            if (!ImGuizmo::IsUsing())
                m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case Saba::KeyCode::E:
            if (!ImGuizmo::IsUsing())
                m_GuizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        case Saba::KeyCode::R:
            if (!ImGuizmo::IsUsing())
                m_GuizmoType = ImGuizmo::OPERATION::SCALE;
            break;
    }
    return false;
}
bool EditorLayer::OnMouseButtonPressed(Saba::MouseButtonPressedEvent& e) {
    switch (e.GetButton()) {
        default:
            break;
        case Saba::MouseCode::ButtonLeft:
            if (m_ViewportHovered && !ImGuizmo::IsOver() && !Saba::Input::IsKeyPressed(Saba::KeyCode::LeftAlt))
                m_SceneHierarchyPanel->SetSelected(m_HoveredEntity);
    }
    return false;
}


void EditorLayer::OnScenePlay() {
    if (!m_EditorScene)
        return;

    m_SceneState = SceneState::Play;
    m_ActiveScene = Saba::Scene::Copy(m_EditorScene);
    m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
    m_SceneHierarchyPanel->SetScene(m_ActiveScene);
}
void EditorLayer::OnSceneStop() {
    m_SceneState = SceneState::Edit;
    m_ActiveScene = m_EditorScene;
    m_SceneHierarchyPanel->SetScene(m_ActiveScene);
}
void EditorLayer::OnDuplicateEntity() {
    if (m_SceneState != SceneState::Edit)
        return;

    Saba::Entity entity = m_SceneHierarchyPanel->GetSelected();
    if (entity)
        m_EditorScene->DuplicateEntity(entity);
}

void EditorLayer::NewScene() {
    m_ActiveScene = Ref<Saba::Scene>::Create();
    m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
    m_SceneHierarchyPanel->SetScene(m_ActiveScene);
    m_EditorScenePath = std::filesystem::path();
}
void EditorLayer::OpenScene() {
    std::filesystem::path filepath = Saba::FileProcessing::ChooseFileToOpenFrom();
    if (filepath.empty())
        return;
    OpenScene(filepath);
}
void EditorLayer::OpenScene(const std::filesystem::path& path) {
    if (m_SceneState != SceneState::Edit)
        OnSceneStop();

    if (!std::filesystem::exists(path)) {
        SB_WARN("Could not open scene from {0}", path.string());
        return;
    }

    Ref<Saba::Scene> scene = Ref<Saba::Scene>::Create();
    Saba::SceneSerializer serializer(scene);
    if (serializer.Deserialize(path)) {
        m_EditorScene = scene;
        m_EditorScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        m_SceneHierarchyPanel->SetScene(m_EditorScene);
        m_ActiveScene = m_EditorScene;
        m_EditorScenePath = path;
    }
}
void EditorLayer::SaveScene() {
    if (m_EditorScenePath.empty())
        SaveSceneAs();
    else {
        Saba::SceneSerializer serializer(m_ActiveScene);
        serializer.Serialize(m_EditorScenePath);
    }
}
void EditorLayer::SaveSceneAs() {
    std::filesystem::path filepath = Saba::FileProcessing::ChooseFileToSaveTo();
    if (filepath.empty())
        return;

    Saba::SceneSerializer serializer(m_ActiveScene);
    serializer.Serialize(filepath);
    m_EditorScenePath = filepath;
}
