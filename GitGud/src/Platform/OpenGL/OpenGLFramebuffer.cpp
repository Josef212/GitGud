#include "ggpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>


namespace GitGud
{
	OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecification specs) : _rendererId(0), _colorAttachment(0), _depthAttachment(0), _specification(specs)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &_rendererId);
		glDeleteTextures(1, &_colorAttachment);
		glDeleteTextures(1, &_depthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (_rendererId)
		{
			glDeleteFramebuffers(1, &_rendererId);
			glDeleteTextures(1, &_colorAttachment);
			glDeleteTextures(1, &_depthAttachment);
		}

		glCreateFramebuffers(1, &_rendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);

		glCreateTextures(GL_TEXTURE_2D, 1, &_colorAttachment);
		glBindTexture(GL_TEXTURE_2D, _colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _specification.Width, _specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &_depthAttachment);
		glBindTexture(GL_TEXTURE_2D, _depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _specification.Width, _specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthAttachment, 0);

		GG_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);
		glViewport(0, 0, _specification.Width, _specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		_specification.Width = width;
		_specification.Height = height;

		Invalidate();
	}
}