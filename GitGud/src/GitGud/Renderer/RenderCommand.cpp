#include "ggpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GitGud
{
	Scope<RendererAPI> RenderCommand::s_rendererAPI = RendererAPI::Create();
}