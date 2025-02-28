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
}
void ExampleLayer::OnDetach() {

}
void ExampleLayer::OnUpdate([[maybe_unused]] Saba::Timestep ts) {
    Saba::Application::Get().GetWindow()->BindToRender();
    Saba::Application::Get().GetWindow()->Clear();

    m_Shader->Bind();
    m_InputLayout->Bind();

    Saba::RenderCommand::Draw(Saba::RendererAPI::Topology::Triangles, 3u);
    Saba::Renderer::Render();
}
void ExampleLayer::OnUIRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Settings");
    ImGui::Text("Frame time: %.3fms (%.1f fps)", static_cast<double>(1000.0f / io.Framerate), static_cast<double>(io.Framerate));
    ImGui::End();
}
void ExampleLayer::OnEvent(Saba::Event& e) {
    Saba::Dispatcher dispatcher(e);
    dispatcher.Dispatch<Saba::MouseButtonPressedEvent>(SB_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
}
bool ExampleLayer::OnMouseButtonPressed([[maybe_unused]] Saba::MouseButtonPressedEvent& e) {
    return false;
}
