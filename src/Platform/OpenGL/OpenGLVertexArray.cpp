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

		GG_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		GG_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &_rendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GG_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &_rendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		GG_PROFILE_FUNCTION();

		glBindVertexArray(_rendererId);
	}

	void OpenGLVertexArray::UnBind() const
	{
		GG_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		GG_PROFILE_FUNCTION();

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

	void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GG_PROFILE_FUNCTION();

		glBindVertexArray(_rendererId);
		indexBuffer->Bind();
		_indexBuffer = indexBuffer;
	}
}
