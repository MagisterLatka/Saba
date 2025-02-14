#pragma once

#include "Saba/Core.h"
#include "Saba/LayerStack.h"

int main(int argc, char** argv, char** envp);

namespace Saba {

struct ApplicationSpecifications
{
    std::string name = "Saba Engine";
};

class Application
{
    friend int ::main(int, char**, char**);
public:
    SB_CORE Application(const ApplicationSpecifications& applicationSpecifications = ApplicationSpecifications());
    SB_CORE virtual ~Application() = default;

    SB_CORE void Close() noexcept;

    SB_CORE Layer* PushLayer(Layer* layer) { return m_LayerStack->PushLayer(layer); }
    SB_CORE Layer* PushOverlay(Layer* overlay) { return m_LayerStack->PushOverlay(overlay); }

    SB_CORE const ApplicationSpecifications& GetApplicationSpecifications() const noexcept { return m_Specs; }

    SB_CORE static Application& Get() noexcept { return *s_Application; }
private:
    SB_CORE void Init();
    SB_CORE void Shutdown();
    SB_CORE int Run();

    SB_CORE void OnEvent(Event& e);
private:
    ApplicationSpecifications m_Specs;

    Scope<LayerStack> m_LayerStack;

    Timer m_Timer;
    Timestep m_Timestep;

    bool m_Running = true, m_Minimized = true;

    SB_CORE static Application* s_Application;
};

Application* CreateApplication();

} //namespace Saba
