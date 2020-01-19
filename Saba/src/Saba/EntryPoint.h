#pragma once

extern Saba::Application* Saba::CreateApplication();

int main()
{
	auto app = Saba::CreateApplication();
	app->Run();
	delete app;
}

