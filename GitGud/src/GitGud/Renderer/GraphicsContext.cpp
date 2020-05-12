#include "ggpch.h"
#include "GitGud/Renderer/GraphicsContext.h"

#include "GitGud/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace GitGud
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		GG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}