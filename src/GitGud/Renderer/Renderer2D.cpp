#include "ggpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GitGud
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TextureId;
		glm::vec2 Tiling;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		const uint32_t MaxTextureCount = 32; // TODO: Assign querying GPU

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> SpriteShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// TODO: Using OpenGL texture id to identify the asset. Need to Change for a unique asset id
		std::array<Ref<Texture2D>, 32> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0: White

		// ---
		glm::vec4 QuadVertexPositions[4]
		{
			{-0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f,  0.5f, 0.0f, 1.0f},
			{-0.5f,  0.5f, 0.0f, 1.0f}
		};
	};

	static Renderer2DData* s_Data;

	void Renderer2D::Init()
	{
		GG_PROFILE_FUNCTION();

		s_Data = new Renderer2DData();

		/*float vertices[4 * 5] =
		{
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
		};*/

		//uint32_t indices[] =
		//{
		//	0, 1, 2, 0, 2, 3
		//};

		s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data->MaxIndices];
		
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data->MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		s_Data->QuadVertexArray = VertexArray::Create();
		
		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data->MaxIndices);
		s_Data->QuadVertexBuffer = VertexBuffer::Create(s_Data->MaxVertices * sizeof(QuadVertex));
		
		delete[] quadIndices;

		BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_position"},
			{ShaderDataType::Float4, "a_color"},
			{ShaderDataType::Float2, "a_texCords"},
			{ShaderDataType::Float, "a_texIndex"},
			{ShaderDataType::Float2, "a_tiling"}
		};

		s_Data->QuadVertexBuffer->SetLayout(layout);
		s_Data->QuadVertexArray->AddVertexBuffer(s_Data->QuadVertexBuffer);
		s_Data->QuadVertexArray->AddIndexBuffer(quadIndexBuffer);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteData, sizeof(uint32_t));

		s_Data->SpriteShader = Shader::Create("assets/shaders/SpriteShader.glsl");
		
		s_Data->SpriteShader->Bind();
		//s_Data->SpriteShader->SetInt("u_texture", 0);
		s_Data->SpriteShader->SetFloat2("u_tiling", glm::vec2(1.0f));

		s_Data->TextureSlots[0] = s_Data->WhiteTexture;
		int32_t samplers[32];
		for (uint32_t i = 0; i < s_Data->MaxTextureCount; ++i)
		{
			samplers[i] = i;
		}

		s_Data->SpriteShader->SetIntArray("u_textures", samplers, 32);
	}

	void Renderer2D::Shutdown()
	{
		GG_PROFILE_FUNCTION();

		delete s_Data->QuadVertexBufferBase;
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		GG_PROFILE_FUNCTION();

		s_Data->SpriteShader->Bind();
		s_Data->SpriteShader->SetMat4("u_vp", camera.GetViewProjectionMatrix());

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
		s_Data->TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		GG_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase;
		s_Data->QuadVertexBuffer->SetData(s_Data->QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		GG_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_Data->TextureSlotIndex; ++i)
		{
			s_Data->TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, s_Data->WhiteTexture, { 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, color, s_Data->WhiteTexture, { 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, angle, color, s_Data->WhiteTexture, { 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		DrawQuad(position, size, angle, color, s_Data->WhiteTexture, { 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, angle, glm::vec4(1.0f), texture, tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		DrawQuad(position, size, angle, glm::vec4(1.0f), texture, tiling);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		const glm::vec2 halfSize = size * 0.5f;
		float textureIndex = GetTextureIndex(texture);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[0];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
		s_Data->QuadVertexBufferPtr->Tiling = tiling;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[1];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
		s_Data->QuadVertexBufferPtr->Tiling = tiling;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[2];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
		s_Data->QuadVertexBufferPtr->Tiling = tiling;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[3];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
		s_Data->QuadVertexBufferPtr->Tiling = tiling;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		float textureIndex = GetTextureIndex(texture);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(angle), { 0.0f, 0.0f, 1.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });
		
		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[0];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
		s_Data->QuadVertexBufferPtr->Tiling = tiling;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[1];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
		s_Data->QuadVertexBufferPtr->Tiling = tiling;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[2];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
		s_Data->QuadVertexBufferPtr->Tiling = tiling;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[3];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
		s_Data->QuadVertexBufferPtr->Tiling = tiling;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadIndexCount += 6;
	}
	
	float Renderer2D::GetTextureIndex(const Ref<Texture2D>& texture)
	{
		if (*s_Data->WhiteTexture.get() == *texture.get())
		{
			return 0.0f; // Just the white texture
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data->TextureSlotIndex; ++i)
		{
			if (*s_Data->TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data->TextureSlotIndex;
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
			s_Data->TextureSlotIndex++;
		}

		return textureIndex;
	}
}
