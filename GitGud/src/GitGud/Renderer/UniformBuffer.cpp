#include "ggpch.h"
#include "UniformBuffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace GitGud
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GG_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		GG_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}
}