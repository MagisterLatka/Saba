#include "pch.h"
#include "Window.h"
#include "Log.h"
#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"
#include "Events\WindowEvent.h"

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

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.eventCallback(e);
					 break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.eventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.eventCallback(e);
					break;
				}
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint key)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent e(key);
			data.eventCallback(e);
		});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button);
					data.eventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.eventCallback(e);
					break;
				}
			}
		});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent e((float)xpos, (float)ypos);
			data.eventCallback(e);
		});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent e((float)xoffset, (float)yoffset);
			data.eventCallback(e);
		});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent e;
			data.eventCallback(e);
		});
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int xsize, int ysize) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = xsize;
			data.height = ysize;

			WindowResizeEvent e(xsize, ysize);
			data.eventCallback(e);
		});
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (focused)
			{
				case GLFW_TRUE:
				{
					WindowFocusEvent e;
					data.eventCallback(e);
					break;
				}
				case GLFW_FALSE:
				{
					WindowLostFocusEvent e;
					data.eventCallback(e);
					break;
				}
			}
		});
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
