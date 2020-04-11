#pragma once

#include <string>
#include <glm/glm.hpp>

namespace GitGud
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string fragSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& value) const;

	private:
		uint _rendererId;
	};
}