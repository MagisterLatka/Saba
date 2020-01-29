#include "pch.h"
#include "InputGLFW.h"
#include "GLFW\glfw3.h"

namespace Saba {

	bool InputGLFW::IsKeyPressedImpl(int key)
	{
		auto state = glfwGetKey(glfwGetCurrentContext(), key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool InputGLFW::IsMouseButtonPressedImpl(int button)
	{
		auto state = glfwGetMouseButton(glfwGetCurrentContext(), button);
		return state == GLFW_PRESS;
	}
	std::pair<float, float> InputGLFW::GetMousePosImpl()
	{
		double xpos, ypos;
		glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}
	float InputGLFW::GetMouseXPosImpl()
	{
		auto [xpos, ypos] = GetMousePosImpl();
		return xpos;
	}
	float InputGLFW::GetMouseYPosImpl()
	{
		auto [xpos, ypos] = GetMousePosImpl();
		return ypos;
	}

}
