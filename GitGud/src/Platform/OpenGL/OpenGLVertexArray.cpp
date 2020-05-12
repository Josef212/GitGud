#include "ggpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace GitGud
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
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
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(_vertexBufferIndex);
				glVertexAttribPointer(
					_vertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				++_vertexBufferIndex;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; ++i)
				{
					glEnableVertexAttribArray(_vertexBufferIndex);
					glVertexAttribPointer(
						_vertexBufferIndex, 
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(sizeof(float) * count * i));
					glVertexAttribDivisor(_vertexBufferIndex, 1);
					++_vertexBufferIndex;
				}
				break;
			}
			default: GG_CORE_ASSERT(false, "Unkown ShaderDataType!");
			}
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
