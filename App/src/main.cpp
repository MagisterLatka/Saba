#include <pch.h>
#include <Saba.h>
#include <EntryPoint.h>
#include "ExampleLayer.h"

Saba::Application* Saba::CreateApplication()
{
    ApplicationSpecifications spec;
    Application* app = new Application(spec);
    [[maybe_unused]] ExampleLayer* layer = static_cast<ExampleLayer*>(app->PushLayer(new ExampleLayer));
    return app;
}
