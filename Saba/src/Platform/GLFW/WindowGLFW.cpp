#include "pch.h"
#include "WindowGLFW.h"

#include "Saba\Events\KeyEvent.h"
#include "Saba\Events\MouseEvent.h"
#include "Saba\Events\WindowEvent.h"

#include <glad\glad.h>

namespace Saba {

	static uint8_t s_Windows = 0;

	static void GLFWErrorCallback(int error, const char* message)
	{
		SB_CORE_ERROR("GLFWError {0}: {1}", error, message);
	}

	WindowGLFW::WindowGLFW(const WindowProps& props)
	{
		Init(props);
	}
	WindowGLFW::~WindowGLFW()
	{
		Shutdown();
	}

	void WindowGLFW::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowGLFW::SetVSync(bool isVSync)
	{
		if (isVSync)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		
		m_Data.vSync = isVSync;
	}
	bool WindowGLFW::IsVSync() const
	{
		return m_Data.vSync;
	}
	void WindowGLFW::SetEventCallback(const EventCallbackFN& func)
	{
		m_Data.eventCallback = func;
	}

	void WindowGLFW::Init(const WindowProps& props)
	{
		m_Data.title = props.Title;
		m_Data.width = props.Width;
		m_Data.height = props.Height;

		if (s_Windows == 0)
		{
			int success = glfwInit();
			SB_CORE_ASSERT(success, "Failed to initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), nullptr, nullptr);
		s_Windows++;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

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
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key)
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
			glViewport(0, 0, xsize, ysize);

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
	void WindowGLFW::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		s_Windows--;
		if (m_Window == 0)
		{
			glfwTerminate();
		}
	}
}	
