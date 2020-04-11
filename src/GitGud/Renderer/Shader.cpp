#include "ggpch.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace GitGud
{
	Shader::Shader(const std::string& vertexSrc, const std::string fragSrc)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* source = (const GLchar*)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertexShader);
			
			GG_CORE_ERROR("Vertex shader compilation failure!");
			GG_CORE_ERROR("{0}", infoLog.data());
			GG_CORE_ASSERT(false, "Vertex share.");
			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		source = (const GLchar*)fragSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			
			GG_CORE_ERROR("Fragment shader compilation failure!");
			GG_CORE_ERROR("{0}", infoLog.data());
			GG_CORE_ASSERT(false, "Fragment share.");
			return;
		}

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			GG_CORE_ERROR("Shader linking failure!");
			GG_CORE_ERROR("{0}", infoLog.data());
			GG_CORE_ASSERT(false, "Shader link.");
			return;
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		_rendererId = program;
	}

	Shader::~Shader()
	{
	}

	void Shader::Bind() const
	{
		glUseProgram(_rendererId);
	}

	void Shader::UnBind() const
	{
		glUseProgram(0);
	}

	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) const
	{
		int loc = glGetUniformLocation(_rendererId, name.c_str());
		glUniform4f(loc, value.r, value.g, value.b, value.a);
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& value) const
	{
		int loc = glGetUniformLocation(_rendererId, name.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}
}