#include "ggpch.h"
#include "Framebuffer.h"

#include "GitGud/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace GitGud
{
	Ref<Framebuffer> Framebuffer::Create(FramebufferSpecification specs)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: GG_CORE_ASSERT(false, "RendererAPI::None is not supported!");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(specs);
		}

		GG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}