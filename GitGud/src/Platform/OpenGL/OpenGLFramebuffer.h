#pragma once

#include "GitGud/Renderer/FrameBuffer.h"

namespace GitGud
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(FramebufferSpecification specs);
		~OpenGLFramebuffer();

		virtual const FramebufferSpecification& GetSpecification() const override { return _specification; }

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererId() override { return _colorAttachment; }

	private:

	private:
		uint32_t _rendererId;
		uint32_t _colorAttachment, _depthAttachment;
		FramebufferSpecification _specification;
	};
}