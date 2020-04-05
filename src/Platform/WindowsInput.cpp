#include "ggpch.h"
#include "WindowsInput.h"
#include "GitGud/Application.h"
#include "GitGud/Window.h"

#include <GLFW/glfw3.h>

namespace GitGud
{
	Input* Input::s_instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return (float)x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return (float)y;
	}
	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float) ypos };
	}
}
