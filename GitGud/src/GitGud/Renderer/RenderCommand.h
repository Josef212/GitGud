#pragma once

#include "RendererAPI.h"

namespace GitGud
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_rendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_rendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_rendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_rendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_rendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_rendererAPI->DrawLines(vertexArray, vertexCount);
		}

		inline static void SetLineWidth(float width)
		{
			s_rendererAPI->SetLineWidth(width);
		}

	private:
		static Scope<RendererAPI> s_rendererAPI;
	};
}