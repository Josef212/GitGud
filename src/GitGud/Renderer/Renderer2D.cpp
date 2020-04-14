#include "ggpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GitGud
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TextureId;
		glm::vec2 Tiling;
	};

	/*struct TriRenderer2DData
	{
		const uint32_t MaxShapes = 10000;
		const uint32_t MaxVertices = MaxShapes * 3;
		const uint32_t MaxIndices = MaxShapes * 3;

		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;

		uint32_t IndexCount = 0;
		Vertex* VertexBufferBase = nullptr;
		Vertex* VertexBufferPtr = nullptr;
	};*/

	struct QuadRenderer2DData
	{
		const uint32_t MaxShapes = 10000;
		const uint32_t MaxVertices = MaxShapes * 4;
		const uint32_t MaxIndices = MaxShapes * 6;

		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;

		uint32_t IndexCount = 0;
		Vertex* VertexBufferBase = nullptr;
		Vertex* VertexBufferPtr = nullptr;
	};

	struct Renderer2DData
	{
		//TriRenderer2DData TriShapeData;
		QuadRenderer2DData QuadShapeData;
				
		Ref<Shader> SpriteShader;
		Ref<Texture2D> WhiteTexture;

		
		static const uint32_t MaxTextureCount = 32; // TODO: Assign querying GPU
		// TODO: Using OpenGL texture id to identify the asset. Need to Change for a unique asset id
		std::array<Ref<Texture2D>, MaxTextureCount> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0: White

		// ---
		glm::vec4 QuadVertexPositions[4]
		{
			{-0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f,  0.5f, 0.0f, 1.0f},
			{-0.5f,  0.5f, 0.0f, 1.0f}
		};

		glm::vec2 QuadVertexUv[4]
		{
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};
	};

	static Renderer2DData* s_Data;

	void Renderer2D::Init()
	{
		GG_PROFILE_FUNCTION();

		s_Data = new Renderer2DData();
		//TriInfoInit();
		QuadInfoInit();

		uint32_t whiteData = 0xffffffff;
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		s_Data->WhiteTexture->SetData(&whiteData, sizeof(uint32_t));
		s_Data->TextureSlots[0] = s_Data->WhiteTexture;

		s_Data->SpriteShader = Shader::Create("assets/shaders/SpriteShader.glsl");
		
		int32_t samplers[s_Data->MaxTextureCount];
		for (uint32_t i = 0; i < s_Data->MaxTextureCount; ++i)
			samplers[i] = i;

		s_Data->SpriteShader->Bind();
		s_Data->SpriteShader->SetIntArray("u_textures", samplers, s_Data->MaxTextureCount);
	}

	void Renderer2D::Shutdown()
	{
		GG_PROFILE_FUNCTION();

		delete s_Data->QuadShapeData.VertexBufferBase;
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		GG_PROFILE_FUNCTION();

		s_Data->SpriteShader->Bind();
		s_Data->SpriteShader->SetMat4("u_vp", camera.GetViewProjectionMatrix());

		//s_Data->TriShapeData.IndexCount = 0;
		//s_Data->TriShapeData.VertexBufferPtr = s_Data->TriShapeData.VertexBufferBase;

		s_Data->QuadShapeData.IndexCount = 0;
		s_Data->QuadShapeData.VertexBufferPtr = s_Data->QuadShapeData.VertexBufferBase;
		s_Data->TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		GG_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data->QuadShapeData.VertexBufferPtr - (uint8_t*)s_Data->QuadShapeData.VertexBufferBase;
		s_Data->QuadShapeData.VertexBuffer->SetData(s_Data->QuadShapeData.VertexBufferBase, dataSize);

		//dataSize = (uint8_t*)s_Data->TriShapeData.VertexBufferPtr - (uint8_t*)s_Data->TriShapeData.VertexBufferBase;
		//s_Data->TriShapeData.VertexBuffer->SetData(s_Data->TriShapeData.VertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		GG_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_Data->TextureSlotIndex; ++i)
			s_Data->TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data->QuadShapeData.VertexArray, s_Data->QuadShapeData.IndexCount);
		//RenderCommand::DrawIndexed(s_Data->TriShapeData.VertexArray, s_Data->TriShapeData.IndexCount);
	}
	
	// ------------------------------------------------------------------------------------------
	// ----- DrawQuad ---------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, color, s_Data->WhiteTexture, { 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		DrawQuad(position, size, angle, color, s_Data->WhiteTexture, { 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		DrawQuad(position, size, glm::vec4(1.0f), texture, tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		DrawQuad(position, size, angle, glm::vec4(1.0f), texture, tiling);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		DrawQuad(transform, color, texture, tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(angle), { 0.0f, 0.0f, -1.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });
		
		DrawQuad(transform, color, texture, tiling);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		float textureIndex = GetTextureIndex(texture);

		for (uint32_t i = 0; i < 4; ++i)
		{
			s_Data->QuadShapeData.VertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadShapeData.VertexBufferPtr->Color = color;
			s_Data->QuadShapeData.VertexBufferPtr->TexCoord = s_Data->QuadVertexUv[i];
			s_Data->QuadShapeData.VertexBufferPtr->TextureId = textureIndex;
			s_Data->QuadShapeData.VertexBufferPtr->Tiling = tiling;
			s_Data->QuadShapeData.VertexBufferPtr++;
		}

		s_Data->QuadShapeData.IndexCount += 6;
	}

	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	
	void Renderer2D::DrawTriangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawTriangle(position, size, color, s_Data->WhiteTexture, { 1.0f, 1.0f });
	}

	void Renderer2D::DrawTriangle(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		DrawTriangle(position, size, angle, color, s_Data->WhiteTexture, { 1.0f, 1.0f });
	}

	void Renderer2D::DrawTriangle(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		DrawTriangle(position, size, glm::vec4(1.0f), texture, tiling);
	}

	void Renderer2D::DrawTriangle(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		DrawTriangle(position, size, angle, glm::vec4(1.0f), texture, tiling);
	}

	void Renderer2D::DrawTriangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		glm::mat4& transform = glm::translate(glm::mat4(1.0f), position);
		DrawTriangle(transform, size, color, texture, tiling);
	}

	void Renderer2D::DrawTriangle(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		glm::mat4& transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, glm::radians(angle), { 0.0f, 0.0f, -1.0f });
		DrawTriangle(transform, size, color, texture, tiling);
	}

	void Renderer2D::DrawTriangle(const glm::mat4& transform, const glm::vec2& size, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		float textureIndex = GetTextureIndex(texture);
		float halfWidth = size.x * 0.5f;

		//s_Data->TriShapeData.VertexBufferPtr->Position = transform * glm::vec4(-halfWidth, 0.0f, 0.0f, 1.0f);
		//s_Data->TriShapeData.VertexBufferPtr->Color = color;
		//s_Data->TriShapeData.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		//s_Data->TriShapeData.VertexBufferPtr->TextureId = textureIndex;
		//s_Data->TriShapeData.VertexBufferPtr->Tiling = tiling;
		//s_Data->TriShapeData.VertexBufferPtr++;
		//
		//s_Data->TriShapeData.VertexBufferPtr->Position = transform * glm::vec4(halfWidth, 0.0f, 0.0f, 1.0f);
		//s_Data->TriShapeData.VertexBufferPtr->Color = color;
		//s_Data->TriShapeData.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		//s_Data->TriShapeData.VertexBufferPtr->TextureId = textureIndex;
		//s_Data->TriShapeData.VertexBufferPtr->Tiling = tiling;
		//s_Data->TriShapeData.VertexBufferPtr++;
		//
		//s_Data->TriShapeData.VertexBufferPtr->Position = transform * glm::vec4(0.0f, size.y, 0.0f, 1.0f);
		//s_Data->TriShapeData.VertexBufferPtr->Color = color;
		//s_Data->TriShapeData.VertexBufferPtr->TexCoord = { 0.5f, 1.0f };
		//s_Data->TriShapeData.VertexBufferPtr->TextureId = textureIndex;
		//s_Data->TriShapeData.VertexBufferPtr->Tiling = tiling;
		//s_Data->TriShapeData.VertexBufferPtr++;
		//
		//s_Data->TriShapeData.IndexCount += 3;
	}

	/*void Renderer2D::TriInfoInit()
	{
		s_Data->TriShapeData.VertexBufferBase = new Vertex[s_Data->TriShapeData.MaxVertices];

		uint32_t* triIndices = new uint32_t[s_Data->TriShapeData.MaxIndices];
		for (uint32_t i = 0, offset = 0; i < s_Data->TriShapeData.MaxIndices; i += 6)
		{
			triIndices[i + 0] = offset + 0;
			triIndices[i + 1] = offset + 1;
			triIndices[i + 2] = offset + 2;

			offset += 3;
		}

		s_Data->TriShapeData.VertexArray = VertexArray::Create();

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(triIndices, s_Data->TriShapeData.MaxIndices);
		s_Data->TriShapeData.VertexBuffer = VertexBuffer::Create(s_Data->TriShapeData.MaxVertices * sizeof(Vertex));

		delete[] triIndices;

		BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_position"},
			{ShaderDataType::Float4, "a_color"},
			{ShaderDataType::Float2, "a_texCords"},
			{ShaderDataType::Float, "a_texIndex"},
			{ShaderDataType::Float2, "a_tiling"}
		};

		s_Data->TriShapeData.VertexBuffer->SetLayout(layout);
		s_Data->TriShapeData.VertexArray->AddVertexBuffer(s_Data->TriShapeData.VertexBuffer);
		s_Data->TriShapeData.VertexArray->AddIndexBuffer(quadIndexBuffer);
	}*/

	void Renderer2D::QuadInfoInit()
	{
		s_Data->QuadShapeData.VertexBufferBase = new Vertex[s_Data->QuadShapeData.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data->QuadShapeData.MaxIndices];
		for (uint32_t i = 0, offset = 0; i < s_Data->QuadShapeData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		s_Data->QuadShapeData.VertexArray = VertexArray::Create();

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data->QuadShapeData.MaxIndices);
		s_Data->QuadShapeData.VertexBuffer = VertexBuffer::Create(s_Data->QuadShapeData.MaxVertices * sizeof(Vertex));

		delete[] quadIndices;

		BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_position"},
			{ShaderDataType::Float4, "a_color"},
			{ShaderDataType::Float2, "a_texCords"},
			{ShaderDataType::Float, "a_texIndex"},
			{ShaderDataType::Float2, "a_tiling"}
		};

		s_Data->QuadShapeData.VertexBuffer->SetLayout(layout);
		s_Data->QuadShapeData.VertexArray->AddVertexBuffer(s_Data->QuadShapeData.VertexBuffer);
		s_Data->QuadShapeData.VertexArray->AddIndexBuffer(quadIndexBuffer);
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
