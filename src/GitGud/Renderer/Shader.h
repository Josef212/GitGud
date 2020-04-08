#pragma once

#include <string>

namespace GitGud
{
	class Shader
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