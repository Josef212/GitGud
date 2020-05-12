#include "ggpch.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GitGud
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None: GG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLRendererAPI>();
		}

		GG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}