#include "ggpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace GitGud
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : _path(path), _width(0), _height(0), _rendererId(0)
	{
		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		void* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		GG_CORE_ASSERT(data, "Failed to load image at path: {0}", path.c_str());

		_width = width;
		_height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
		glTextureStorage2D(_rendererId, 1, GL_RGB8, _width, _height);

		glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &_rendererId);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, _rendererId);
	}
}
