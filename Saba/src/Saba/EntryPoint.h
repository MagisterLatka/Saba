#pragma once

extern Saba::Application* Saba::CreateApplication();

int main()
{
	Saba::Log::Init();
	SB_CORE_INFO("Initialized Log");

	auto app = Saba::CreateApplication();
	app->Run();
	delete app;
}

