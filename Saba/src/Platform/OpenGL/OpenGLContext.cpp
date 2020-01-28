#include "pch.h"
#include "OpenGLContext.h"

#include <GLFW\glfw3.h>
#include <glad\glad.h>

namespace Saba {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SB_CORE_ASSERT(success, "Failed to initialize GLAD!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}
