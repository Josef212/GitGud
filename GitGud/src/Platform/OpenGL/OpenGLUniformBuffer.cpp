#include "ggpch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace GitGud
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &_rendererId);
		glNamedBufferData(_rendererId, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, _rendererId);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &_rendererId);
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(_rendererId, offset, size, data);
	}
}