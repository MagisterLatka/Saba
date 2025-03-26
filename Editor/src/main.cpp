#include <pch.h>
#include <Saba.h>
#include <EntryPoint.h>
#include "EditorLayer.h"

#include <imgui.h>

Saba::Application* Saba::CreateApplication()
{
    ApplicationSpecifications spec;
    spec.Width = 1600u;
    spec.Height = 900u;
    spec.WindowIconPath = "assets/Icon.png";
    spec.CustomTitleBar = true;
    
    auto* app = new Application(spec);
    auto* layer = static_cast<EditorLayer*>(app->PushLayer(new EditorLayer));
    app->SetMenuBarCallback([app, layer]() {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N"))
                layer->NewScene();
            if (ImGui::MenuItem("Save", "Ctrl+S"))
                layer->SaveScene();
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
                layer->SaveSceneAs();
            if (ImGui::MenuItem("Open", "Ctrl+O"))
                layer->OpenScene();

            if (ImGui::MenuItem("Close"))
                app->Close();

            ImGui::EndMenu();
        }
    });
    return app;
}
