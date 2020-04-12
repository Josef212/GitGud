#include "ggpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace GitGud
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : _path(""), _width(width), _height(height), _rendererId(0), _internalFormat(GL_RGBA8), _dataFormat(GL_RGBA)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
		glTextureStorage2D(_rendererId, 1, _internalFormat, _width, _height);

		glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : _path(path), _width(0), _height(0), _rendererId(0), _internalFormat(0), _dataFormat(0)
	{
		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		void* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		GG_CORE_ASSERT(data, "Failed to load image at path: {0}", path.c_str());

		_width = width;
		_height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		GG_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		_internalFormat = internalFormat;
		_dataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
		glTextureStorage2D(_rendererId, 1, _internalFormat, _width, _height);

		glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &_rendererId);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = _dataFormat == GL_RGBA ? 4 : 3;
		GG_CORE_ASSERT(size == _width * _height * bpp, "Data size do not match texture size!");

		glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, _rendererId);
	}
}
