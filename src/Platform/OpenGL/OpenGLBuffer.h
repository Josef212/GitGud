#pragma once

#include "GitGud/Renderer/Buffer.h"

namespace GitGud
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, size_t size);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const BufferLayout& GetLayout() const override { return _layout; }
		virtual void SetLayout(const BufferLayout& layout) override { _layout = layout; }

	private:
		uint32_t _rendererId;
		BufferLayout _layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual uint32_t GetCount() const override { return _count; }

	private:
		uint32_t _rendererId;
		uint32_t _count;
	};
}
