#pragma once

#include "GitGud/Renderer/Texture.h"

#include <glad/glad.h>

namespace GitGud
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return _width; }
		virtual uint32_t GetHeight() const override { return _height; }

		virtual uint32_t GetRendererID() const override { return _rendererId; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;
		
		virtual bool operator==(const Texture& other) const override
		{
			return _rendererId == ((OpenGLTexture2D&)other)._rendererId;
		}

	private:
		std::string _path;
		uint32_t _width, _height;
		uint32_t _rendererId;

		GLenum _internalFormat, _dataFormat;
	};
}