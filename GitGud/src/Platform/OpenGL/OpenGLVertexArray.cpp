#include "ggpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace GitGud
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case GitGud::ShaderDataType::Float:		return GL_FLOAT;
			case GitGud::ShaderDataType::Float2:	return GL_FLOAT;
			case GitGud::ShaderDataType::Float3:	return GL_FLOAT;
			case GitGud::ShaderDataType::Float4:	return GL_FLOAT;
			case GitGud::ShaderDataType::Mat3:		return GL_FLOAT;
			case GitGud::ShaderDataType::Mat4:		return GL_FLOAT;
			case GitGud::ShaderDataType::Int:		return GL_INT;
			case GitGud::ShaderDataType::Int2:		return GL_INT;
			case GitGud::ShaderDataType::Int3:		return GL_INT;
			case GitGud::ShaderDataType::Int4:		return GL_INT;
			case GitGud::ShaderDataType::Bool:		return GL_BOOL;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &_rendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &_rendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(_rendererId);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		GG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(_rendererId);
		vertexBuffer->Bind();
				
		const auto& layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			++index;
		}

		_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(_rendererId);
		indexBuffer->Bind();
		_indexBuffer = indexBuffer;
	}
}
