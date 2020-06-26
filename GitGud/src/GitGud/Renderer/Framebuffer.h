#pragma once

#include "GitGud/Core/Core.h"

namespace GitGud
{
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		// FrambufferFormat Format;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(FramebufferSpecification specs);

	public:
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererId() = 0;
	};
}