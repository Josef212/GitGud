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

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual uint32_t GetColorAttachmentRendererId(uint32_t index = 0) override { GG_CORE_ASSERT(index < _colorAttachments.size(), "Index out of bounds."); return _colorAttachments[index]; }

		virtual void ClearColorAttachment(uint32_t attachmentIndex, int value) override;

	private:

	private:
		uint32_t _rendererId;
		FramebufferSpecification _specification;

		std::vector<FramebufferTextureSpecification> _colorAttachmentSpecs;
		FramebufferTextureSpecification _depthAttachmentSpec = FramebufferTextureFormat::None;

		std::vector<uint32_t> _colorAttachments;
		uint32_t _depthAttachment = 0;
	};
}