#include "pch.h"
#include "Platform/GLFW/WindowGLFW.h"

#include "Saba/Events/KeyEvent.h"
#include "Saba/Events/MouseEvent.h"
#include "Saba/Events/WindowEvent.h"

#include <glad/glad.h>

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
		m_Context->SwapBuffers();
		glfwPollEvents();
	}

	void WindowGLFW::SetVSync(bool isVSync)
	{
		glfwSwapInterval(isVSync ? 1 : 0);
		m_Data.vSync = isVSync;
	}
	bool WindowGLFW::IsVSync() const
	{
		return m_Data.vSync;
	}
	void WindowGLFW::EnableCursor(bool enable)
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, enable ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
	bool WindowGLFW::IsCursorEnabled() const
	{
		return glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
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
		SB_CORE_ASSERT(m_Window, "Could not create window!");
		s_Windows++;
		SB_CORE_INFO("Created window {0} ({1}, {2})", m_Data.title, m_Data.width, m_Data.height);

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		glfwGetWindowPos(m_Window, &m_Data.windowPosX, &m_Data.windowPosY);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(static_cast<KeyCode>(key), 0);
					data.eventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(static_cast<KeyCode>(key));
					data.eventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(static_cast<KeyCode>(key), 1);
					data.eventCallback(e);
					break; 
				}
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent e(static_cast<KeyCode>(key));
			data.eventCallback(e);
		});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(static_cast<MouseCode>(button));
					data.eventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(static_cast<MouseCode>(button));
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
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.windowPosX = xpos;
			data.windowPosY = ypos;
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
