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


		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)override;
		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return _vertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return _indexBuffer; }

	private:
		uint32_t _rendererId;
		std::vector<std::shared_ptr<VertexBuffer>> _vertexBuffers;
		std::shared_ptr<IndexBuffer> _indexBuffer;
	};
}
