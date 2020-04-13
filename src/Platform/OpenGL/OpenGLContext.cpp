#include "ggpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace GitGud
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : _windowHandle(windowHandle)
	{
		GG_CORE_ASSERT(_windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		GG_PROFILE_FUNCTION();

		glfwMakeContextCurrent(_windowHandle);
		int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GG_CORE_ASSERT(gladStatus, "Failed to initialize Glad!");

		GG_CORE_INFO("OpenGL Renderer:");
		GG_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		GG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		GG_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

#ifdef GG_ENABLE_ASSERTS
		int major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		GG_CORE_ASSERT(major > 4 || (major == 4 && minor >= 5), "GitGud requires at least OpenGL version 4.5");
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		GG_PROFILE_FUNCTION();

		glfwSwapBuffers(_windowHandle);
	}
}