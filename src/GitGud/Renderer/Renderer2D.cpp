#include "ggpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GitGud
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		float vertices[4 * 5] =
		{
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
		};

		uint32_t indices[] =
		{
			0, 1, 2, 0, 2, 3
		};

		s_Data->QuadVertexArray = VertexArray::Create();
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_position"},
			{ShaderDataType::Float2, "a_texCords"}
		};

		vertexBuffer->SetLayout(layout);
		s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);
		s_Data->QuadVertexArray->AddIndexBuffer(indexBuffer);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_vp", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_vp", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, 0.0f, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, 0.0f, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, angle, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(angle), { 0.0f, 0.0f, 1.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_color", color);
		s_Data->FlatColorShader->SetMat4("u_model", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, angle, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(angle), { 0.0f, 0.0f, 1.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_model", transform);

		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}
