#include "ggpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GitGud
{
	RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI();
}