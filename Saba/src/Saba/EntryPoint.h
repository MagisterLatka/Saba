#pragma once

extern Saba::Application* Saba::CreateApplication();

int main(int argc, char** argv)
{
	Saba::Log::Init();
	SB_CORE_INFO("Initialized Log");

	auto app = Saba::CreateApplication();
	app->Run();
	delete app;
}

