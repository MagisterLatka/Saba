#include <pch.h>
#include <Saba.h>
#include "ExampleLayer.h"

#include <Saba/ImGui/ImGui.h>
#include <Saba/ImGui/ImGuizmo.h>

class CameraController : public Saba::ScriptableEntity {
public:
    CameraController() = default;
    ~CameraController() = default;

    void OnCreate() override {
        m_Camera = GetComponent<Saba::CameraComponent>().Camera.As<Saba::OrthographicCamera>();
    }
    void OnDestroy() override {
        m_Camera.Reset();
    }
    void OnEvent(Saba::Event& e) override {
        Saba::Dispatcher dispatcher(e);
        dispatcher.Dispatch<Saba::MouseScrolledEvent>(SB_BIND_EVENT_FN(CameraController::OnMouseScroll));
    }
    bool OnMouseScroll(Saba::MouseScrolledEvent& e) {
        m_Camera->SetSize(m_Camera->GetSize() * (1 - e.GetYOffset() * 0.1f));
        return false;
    }
    void OnUpdate(Saba::Timestep ts) override {
        const float cameraSize = m_Camera->GetSize();
        float deltaSpeed = p_Speed * static_cast<float>(ts);
        if (cameraSize > 1.0f)
            deltaSpeed *= glm::pow(1.01f, cameraSize) - 0.01f;

        auto delta = glm::vec3(0.0f);
        if (Saba::Input::IsKeyPressed(SB_KEY_UP))
            delta.y = deltaSpeed;
        else if (Saba::Input::IsKeyPressed(SB_KEY_DOWN))
            delta.y = -deltaSpeed;

        if (Saba::Input::IsKeyPressed(SB_KEY_LEFT))
            delta.x = -deltaSpeed;
        else if (Saba::Input::IsKeyPressed(SB_KEY_RIGHT))
            delta.x = deltaSpeed;

        GetTransformComponent().Position += delta;
    }
    void OnViewportResize(uint32_t width, uint32_t height) override {
        m_Camera->SetViewportSize(width, height);
    }
public:
    float p_Speed = 5.0f;
private:
    Ref<Saba::OrthographicCamera> m_Camera;
};

ExampleLayer::ExampleLayer() {

}
ExampleLayer::~ExampleLayer() {

}

void ExampleLayer::OnAttach() {
    auto window = Saba::Application::Get().GetWindow();
    m_RenderPass = Saba::RenderPass::Create();
    m_RenderPass->SetRenderTarget(0u, Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight()));
    m_RenderPass->SetRenderTarget(1u, Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight(), Saba::RenderTargetFormat::R32_UINT));
    m_RenderPass->SetDepthStencilTarget(Saba::RenderTarget::Create(window->GetWidth(), window->GetHeight(), Saba::RenderTargetFormat::Depth32F));

    Saba::Texture2DProps textureProps;
    textureProps.Filepath = "assets/textures/checkerboard.png";
    textureProps.Sampling = Saba::TextureSampling::Point;
    m_Texture = Saba::Texture2D::Create(std::move(textureProps));

    m_Scene = Ref<Saba::Scene>::Create("App scene");
    m_Scene->OnViewportResize(window->GetWidth(), window->GetHeight());
    m_Camera = m_Scene->CreateAndSetCameraEntity();
    m_Camera.GetComponent<Saba::CameraComponent>().Camera.As<Saba::OrthographicCamera>()->SetSize(2.0f);
    m_Camera.AddComponent<Saba::NativeScriptComponent>().Bind<CameraController>();

    m_Scene->CreateEntity("Textured quad").AddComponent<Saba::SpriteComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), m_Texture);

    m_SceneHierarchyPanel = CreateScope<Saba::SceneHierarchyPanel>(m_Scene);
    m_ContentBrowserPanel = CreateScope<Saba::ContentBrowserPanel>();
}
void ExampleLayer::OnDetach() {
    m_ContentBrowserPanel.reset();
    m_SceneHierarchyPanel.reset();
    m_Scene.Reset();
    m_RenderPass.Reset();
    m_Texture.Reset();
}
void ExampleLayer::OnUpdate([[maybe_unused]] Saba::Timestep ts) {
    Saba::Application::Get().GetWindow()->BindWindow();

    if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0 && (m_ViewportSize.x != m_RenderPass->GetWidth() || m_ViewportSize.y != m_RenderPass->GetHeight())) {
        m_RenderPass->Resize(m_ViewportSize.x, m_ViewportSize.y);
        m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    m_RenderPass->Bind();
    m_RenderPass->Clear();
    Saba::Renderer2D::ResetStats();

    m_Scene->OnUpdate(ts);

    glm::ivec2 mousePos = Saba::Input::GetMousePos();
    mousePos -= m_ViewportPos;
    if (mousePos.x > 0 && mousePos.y >= 0 && mousePos.x < static_cast<int>(m_ViewportSize.x) && mousePos.y < static_cast<int>(m_ViewportSize.y)) {
        static uint32_t data = 0u;
        m_RenderPass->GetRenderTarget(1u)->ReadPixel(&data, static_cast<uint32_t>(mousePos.x), static_cast<uint32_t>(mousePos.y));
        Saba::Renderer::Submit([this]() {
            m_HoveredEntity = (data == 0 || data == std::numeric_limits<uint32_t>::max()) ? Saba::Entity() : Saba::Entity(static_cast<entt::entity>(data), m_Scene.Raw());
        });
    }

    Saba::Application::Get().GetWindow()->BindToRender();
    Saba::Application::Get().GetWindow()->Clear();
}
void ExampleLayer::OnUIRender() {
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

    if (Saba::Entity selected = m_SceneHierarchyPanel->GetSelected(); m_GuizmoType != -1 && selected) {
        const bool snap = Saba::Input::IsKeyPressed(Saba::KeyCode::LeftControl);
        const float snapValue = m_GuizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : m_GuizmoType == ImGuizmo::OPERATION::SCALE ? 0.5f : 0.1f;
        const float snapValues[3] = { snapValue, snapValue, snapValue };

        ImGuizmo::SetOrthographic(true);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        const glm::mat4 viewMat = glm::inverse(m_Camera.GetTransform());
        const glm::mat4& projMat = m_Camera.GetComponent<Saba::CameraComponent>().Camera->GetProjectionMatrix();
        glm::mat4 transform = selected.GetTransform();

        ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projMat), static_cast<ImGuizmo::OPERATION>(m_GuizmoType), ImGuizmo::LOCAL,
            glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing()) {
            auto& tc = selected.GetTransformComponent();
            glm::vec3 pos, size, skew;
            glm::quat orientation;
            glm::vec4 perspective;
            glm::decompose(transform, size, orientation, pos, skew, perspective);
            tc.Position = pos;
            tc.Orientation += glm::eulerAngles(orientation) - tc.Orientation;
            tc.Size = size;
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();

    m_SceneHierarchyPanel->OnUIRender();
    m_ContentBrowserPanel->OnUIRender();
}
void ExampleLayer::OnEvent(Saba::Event& e) {
    Saba::Dispatcher dispatcher(e);
    dispatcher.Dispatch<Saba::KeyPressedEvent>(SB_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
    dispatcher.Dispatch<Saba::MouseButtonPressedEvent>(SB_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));

    m_Scene->OnEvent(e);
}
bool ExampleLayer::OnKeyPressed(Saba::KeyPressedEvent& e) {
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
            if (control && shift)
                SaveScene();
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
bool ExampleLayer::OnMouseButtonPressed(Saba::MouseButtonPressedEvent& e) {
    switch (e.GetButton()) {
        default:
            break;
        case Saba::MouseCode::ButtonLeft:
            if (m_ViewportHovered && !ImGuizmo::IsOver())
                m_SceneHierarchyPanel->SetSelected(m_HoveredEntity);
    }
    return false;
}

void ExampleLayer::NewScene() {
    m_Scene = Ref<Saba::Scene>::Create();
    m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
    m_SceneHierarchyPanel->SetScene(m_Scene);
}
void ExampleLayer::OpenScene() {
    std::filesystem::path filepath = Saba::FileProcessing::ChooseFileToOpenFrom();
    if (filepath.empty())
        return;
    OpenScene(filepath);
}
void ExampleLayer::OpenScene(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path))
        return;

    NewScene();

    Saba::SceneSerializer serializer(m_Scene);
    serializer.Deserialize(path);
}
void ExampleLayer::SaveScene() {
    std::filesystem::path filepath = Saba::FileProcessing::ChooseFileToSaveTo();
    if (filepath.empty())
        return;

    Saba::SceneSerializer serializer(m_Scene);
    serializer.Serialize(filepath);
}
