#pragma once

namespace Saba {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		static Application* Get() { return s_Application; }
	private:
		static Application* s_Application;
	};

	Application* CreateApplication();

}
