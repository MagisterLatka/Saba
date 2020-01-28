#include "pch.h"
#include <Saba.h>
#include "ExampleLayer.h"
#include <imgui/imgui.h>

ExampleLayer::ExampleLayer()
	: Saba::Layer("ExampleLayer")
{

}
ExampleLayer::~ExampleLayer()
{

}

void ExampleLayer::OnAttach()
{
}
void ExampleLayer::OnDetach()
{
}
void ExampleLayer::OnEvent(Saba::Event& event)
{
}
void ExampleLayer::OnUpdate()
{
}
void ExampleLayer::OnImGuiRender()
{
	ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
	bool vsync = Saba::Application::Get()->GetWindow().IsVSync();
	ImGui::Checkbox("Set VSync", &vsync);
	Saba::Application::Get()->GetWindow().SetVSync(vsync);
}
