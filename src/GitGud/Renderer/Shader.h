#pragma once

#include "GitGud/Core.h"

#include <string>

namespace GitGud
{
	class GG_API Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string fragSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

	private:
		uint _rendererId;
	};
}