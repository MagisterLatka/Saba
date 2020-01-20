#include "pch.h"
#include "Window.h"
#include "Log.h"

namespace Saba {

	static bool s_Initialized = false;
	static int s_Windows = 0;

	static void GLFWErrorCallback(int error, const char* message)
	{
		SB_CORE_ERROR("GLFWError {0}: {1}", error, message);
	}

	Window::Window(const char* title, int width, int height)
	{
		Init(title, width, height);
		s_Windows++;
	}
	Window::~Window()
	{
		s_Windows--;
		Shutdown();
	}

	void Window::OnUpdate()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void Window::SetVSync(bool isVSync)
	{
		if (isVSync)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		
		m_Data.vSync = isVSync;
	}
	void Window::SetEventCallback(const std::function<void(Event&)>& func)
	{
		m_Data.eventCallback = func;
	}

	void Window::Init(const char* title, int width, int height)
	{
		m_Data.width = width;
		m_Data.height = height;

		if (!s_Initialized)
		{
			int success = glfwInit();
			SB_CORE_ASSERT(success, "Failed to initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_Initialized = true;
		}

		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}
	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		if (m_Window == 0)
		{
			s_Initialized = false;
			glfwTerminate();
		}
	}
}
