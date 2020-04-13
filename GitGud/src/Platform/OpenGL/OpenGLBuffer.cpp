#include "ggpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

GitGud::OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
{
	GG_PROFILE_FUNCTION();

	glCreateBuffers(1, &_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

GitGud::OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size)
{
	GG_PROFILE_FUNCTION();

	glCreateBuffers(1, &_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

GitGud::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	GG_PROFILE_FUNCTION();

	glDeleteBuffers(1, &_rendererId);
}

void GitGud::OpenGLVertexBuffer::Bind() const
{
	GG_PROFILE_FUNCTION();

	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
}

void GitGud::OpenGLVertexBuffer::UnBind() const
{
	GG_PROFILE_FUNCTION();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GitGud::OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
{
	GG_PROFILE_FUNCTION();

	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

// ===============================================================================================
// ===============================================================================================
// ===============================================================================================

GitGud::OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : _count(count)
{
	GG_PROFILE_FUNCTION();

	glCreateBuffers(1, &_rendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

GitGud::OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	GG_PROFILE_FUNCTION();

	glDeleteBuffers(1, &_rendererId);
}

void GitGud::OpenGLIndexBuffer::Bind() const
{
	GG_PROFILE_FUNCTION();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
}

void GitGud::OpenGLIndexBuffer::UnBind() const
{
	GG_PROFILE_FUNCTION();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
