#include "ggpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace GitGud
{
	VertexBuffer* VertexBuffer::Create(float* vertices, size_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None: GG_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		GG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, size_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: GG_CORE_ASSERT(false, "RendererAPI::None is not supported!");
		case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}

		GG_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
