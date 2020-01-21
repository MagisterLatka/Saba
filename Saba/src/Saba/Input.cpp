#include "pch.h"
#include "Input.h"
#include "GLFW\glfw3.h"

namespace Saba {

	bool Input::IsKeyPressed(int key)
	{
		auto state = glfwGetKey(glfwGetCurrentContext(), key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(int button)
	{
		auto state = glfwGetMouseButton(glfwGetCurrentContext(), button);
		return state == GLFW_PRESS;
	}
	std::pair<float, float> Input::GetMousePos()
	{
		double xpos, ypos;
		glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}
	float Input::GetMouseXPos()
	{
		auto [xpos, ypos] = GetMousePos();
		return xpos;
	}
	float Input::GetMouseXYos()
	{
		auto [xpos, ypos] = GetMousePos();
		return ypos;
	}

}
