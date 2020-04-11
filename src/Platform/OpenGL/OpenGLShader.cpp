#include "ggpch.h"
#include "OpenGLShader.h"

#include <fstream>

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace GitGud
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		GG_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string fileSrc = ReadFile(filePath);
		auto shaderSources = PreProcess(fileSrc);
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string fragSrc)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(_rendererId);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(_rendererId);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) const
	{
		int loc = glGetUniformLocation(_rendererId, name.c_str());
		glUniform1i(loc, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value) const
	{
		int loc = glGetUniformLocation(_rendererId, name.c_str());
		glUniform1f(loc, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) const
	{
		int loc = glGetUniformLocation(_rendererId, name.c_str());
		glUniform2f(loc, value.r, value.g);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) const
	{
		int loc = glGetUniformLocation(_rendererId, name.c_str());
		glUniform3f(loc, value.r, value.g, value.b);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) const
	{
		int loc = glGetUniformLocation(_rendererId, name.c_str());
		glUniform4f(loc, value.r, value.g, value.b, value.a);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value) const
	{
		int loc = glGetUniformLocation(_rendererId, name.c_str());
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value) const
	{
		int loc = glGetUniformLocation(_rendererId, name.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			GG_CORE_ERROR("Could not open shader file: '{0}'", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLenght = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			GG_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLenght + 1;
			std::string type = source.substr(begin, eol - begin);
			GG_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified.");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLuint> glShaderIds(shaderSources.size());

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceStr = source.c_str();
			glShaderSource(shader, 1, &sourceStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				GG_CORE_ERROR("{0}", infoLog.data());
				GG_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIds.push_back(shader);
		}

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

			for (auto id : glShaderIds)
				glDeleteShader(id);

			GG_CORE_ERROR("Shader linking failure!");
			GG_CORE_ERROR("{0}", infoLog.data());
			GG_CORE_ASSERT(false, "Shader link.");
			return;
		}

		for (auto id : glShaderIds)
			glDetachShader(program, id);

		_rendererId = program;
	}
}