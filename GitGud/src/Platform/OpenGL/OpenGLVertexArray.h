#pragma once

#include "GitGud/Renderer/VertexArray.h"

namespace GitGud
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void UnBind() const override;


		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)override;
		virtual void AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return _vertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return _indexBuffer; }

	private:
		uint32_t _rendererId;
		uint32_t _vertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> _vertexBuffers;
		Ref<IndexBuffer> _indexBuffer;
	};
}
