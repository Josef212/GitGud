#pragma once

#include "GitGud/Renderer/UniformBuffer.h"

namespace GitGud
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t _rendererId = 0;
	};
}
