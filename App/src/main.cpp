#include <pch.h>
#include <Saba.h>
#include <EntryPoint.h>
#include "ExampleLayer.h"

Saba::Application* Saba::CreateApplication() {
    ApplicationSpecifications spec;
    spec.Width = 1600u;
    spec.Height = 900u;
    spec.WindowIconPath = "assets/Icon.png";
    spec.CustomTitleBar = true;
    spec.DrawUI = false;

    auto* app = new Application(spec);
    app->PushLayer(new ExampleLayer);
    return app;
}
