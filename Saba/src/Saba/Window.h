#pragma once

#include <GLFW\glfw3.h>

#include "Events\Event.h"

namespace Saba {

	class Window
	{
	public:
		Window(const char* title, int width, int height);
		~Window();

		void OnUpdate();

		inline int GetWidth() const { return m_Data.width; }
		inline int GetHeight() const { return m_Data.height; }

		inline bool IsVSync() const { return m_Data.vSync; }
		void SetVSync(bool isVSync);

		void SetEventCallback(const std::function<void(Event&)>& func);
	private:
		void Init(const char* title, int width, int height);
		void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			int width, height;
			bool vSync;

			std::function<void(Event&)> eventCallback;
		};
		WindowData m_Data;
	};

}
