#pragma once
// #include "valgrind/callgrind.h"

extern Saba::Application* Saba::CreateApplication();

int main(int argc, char** argv, char** envp) {
    Saba::InitializeCore(argc, argv, envp);

    Saba::Application* app = nullptr;
    int returnVal = 0;
    try {
        app = Saba::CreateApplication();
        SB_CORE_ASSERT(app, "Could not initialize app!");
        app->Init();
        ImGui::SetCurrentContext(app->m_ImGuiLayer->GetContext());
        // CALLGRIND_START_INSTRUMENTATION;
        // CALLGRIND_TOGGLE_COLLECT;
        returnVal = app->Run();
        // CALLGRIND_TOGGLE_COLLECT;
        // CALLGRIND_STOP_INSTRUMENTATION;
        app->Shutdown();
    }
#if defined(SB_PLATFORM_WINDOWS)
    catch (const Saba::SabaException& e) {
        MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...) {
        MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
#else
    catch (const Saba::SabaException& e) {
        SB_CORE_CRITICAL(e.GetType());
    }
    catch (const std::exception& e) {
        SB_CORE_CRITICAL("Standard Exception");
    }
    catch (...) {
        SB_CORE_CRITICAL("Unknown Exception");
    }
#endif
    if (app)
        delete app;

    Saba::ShutdownCore();
    return returnVal;
}
