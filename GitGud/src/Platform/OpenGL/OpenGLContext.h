#pragma once

#include "GitGud/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace GitGud
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* _windowHandle;
	};
}