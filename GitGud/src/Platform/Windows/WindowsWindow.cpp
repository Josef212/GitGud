#include "ggpch.h"
#include "WindowsWindow.h"

#include "GitGud/Events/ApplicationEvent.h"
#include "GitGud/Events/KeyEvent.h"
#include "GitGud/Events/MouseEvent.h"

#include "GitGud/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace GitGud
{
	static uint8_t s_glfwWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		GG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		GG_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		GG_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		GG_PROFILE_FUNCTION();

		glfwPollEvents();
		_context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		GG_PROFILE_FUNCTION();

		glfwSwapInterval(enabled ? 1 : 0);
		_data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return _data.VSync;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		GG_PROFILE_FUNCTION();

		_data.Title = props.Title;
		_data.Width = props.Width;
		_data.Height = props.Height;
		
		GG_CORE_INFO("Creating window {0} ({1} x {2})", _data.Title, _data.Width, _data.Height);

		if (s_glfwWindowCount == 0)
		{
			GG_PROFILE_SCOPE("glfwInit");
			bool ret = glfwInit();
			GG_CORE_ASSERT(ret, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			GG_PROFILE_SCOPE("glfwCreateWindow");
#if defined(GG_DEBUG)
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			{
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			}
#endif
			_window = glfwCreateWindow(_data.Width, _data.Height, _data.Title.c_str(), nullptr, nullptr);
			++s_glfwWindowCount;
		}
		
		_context = GraphicsContext::Create(_window);
		_context->Init();

		glfwSetWindowUserPointer(_window, &_data);
		SetVSync(true);

		// =================================

		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int widht, int height)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = widht;
				data.Height = height;

				WindowResizeEvent event(widht, height);
				data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
					{
						KeyPressEvent event(key, 0);
						data.EventCallback(event);
					}
					break;
				case GLFW_RELEASE:
					{
						KeyReleaseEvent event(key);
						data.EventCallback(event);
					}
					break;
				case GLFW_REPEAT:
					{
						KeyPressEvent event(key, 1);
						data.EventCallback(event);
					}
					break;
				}
		});

		glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keyCode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypeEvent event(keyCode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
					{
						MouseButtonPressEvent event(button);
						data.EventCallback(event);
					}
					break;
				case GLFW_RELEASE:
					{
						MouseButtonReleaseEvent event(button);
						data.EventCallback(event);
					}
					break;
				}
		});

		glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrollEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMoveEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		GG_PROFILE_FUNCTION();

		glfwDestroyWindow(_window);
		--s_glfwWindowCount;

		if (s_glfwWindowCount == 0)
		{
			glfwTerminate();
		}
	}
}
