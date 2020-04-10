#include "ggpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace GitGud
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: GG_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
		}

		GG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}