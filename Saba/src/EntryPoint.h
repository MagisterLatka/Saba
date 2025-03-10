#pragma once

extern Saba::Application* Saba::CreateApplication();

int main(int argc, char** argv, char** envp)
{
    Saba::InitializeCore(argc, argv, envp);

    Saba::Application* app = nullptr;
    int returnVal = 0;
    try {
        app = Saba::CreateApplication();
        SB_CORE_ASSERT(app, "Could not initialize app!");
        app->Init();
        ImGui::SetCurrentContext(app->m_ImGuiLayer->GetContext());
        returnVal = app->Run();
        app->Shutdown();
    }
#if defined(SB_PLATFORM_WINDOWS)
    catch (const Saba::SabaException& e)
    {
        MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
#else
    catch (const Saba::SabaException& e)
    {
        SB_CORE_CRITICAL(e.GetType());
        SB_CORE_ERROR(e.what());
    }
    catch (const std::exception& e)
    {
        SB_CORE_CRITICAL("Standard Exception");
        SB_CORE_ERROR(e.what());
    }
    catch (...)
    {
        SB_CORE_CRITICAL("Unknown Exception");
        SB_CORE_ERROR("No details available");
    }
#endif
    if (app) delete app;

    Saba::ShutdownCore();
    return returnVal;
}
