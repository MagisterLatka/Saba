#include <pch.h>
#include <Saba.h>
#include <EntryPoint.h>
#include "ExampleLayer.h"

#include <imgui.h>

Saba::Application* Saba::CreateApplication()
{
    ApplicationSpecifications spec;
    spec.WindowIconPath = "assets/Icon.png";
    spec.CustomTitleBar = true;
    auto* app = new Application(spec);
    [[maybe_unused]] auto* layer = static_cast<ExampleLayer*>(app->PushLayer(new ExampleLayer));
    app->SetMenuBarCallback([app]() {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Close"))
                app->Close();

            ImGui::EndMenu();
        }
    });
    return app;
}
