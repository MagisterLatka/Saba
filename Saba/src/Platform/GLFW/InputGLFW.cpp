#include "pch.h"
#include "Saba/Input.h"
#include "Saba/Application.h"

#include <GLFW/glfw3.h>

namespace Saba {

	bool Input::IsKeyPressed(KeyCode key)
	{
		auto state = glfwGetKey(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), static_cast<int>(key));
		return (state == GLFW_PRESS || state == GLFW_REPEAT);
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), static_cast<int>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePos()
	{
		double x, y;
		glfwGetCursorPos(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), &x, &y);
		return { (float)x, (float)y };
	}
	float Input::GetMouseXPos()
	{
		return GetMousePos().x;
	}
	float Input::GetMouseYPos()
	{
		return GetMousePos().y;
	}
}
