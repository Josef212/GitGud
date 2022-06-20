#include "ggpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "UniformBuffer.h"
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

		// Editor
		int EntityId = 0;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor
		int EntityId = 0;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor
		int EntityId = 0;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;

		static const uint32_t MaxTextureSlots = 32; // TODO: Assign querying GPU

		// Quad / Sprite rendering
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> SpriteShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// Circle
		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		// Line
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		// Default white texture
		Ref<Texture2D> WhiteTexture;

		// TODO: Using OpenGL texture id to identify the asset. Need to Change for a unique asset id
		// Textures
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
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

		Renderer2D::Statistics Stats;

		// Camera
		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData* s_Data;

	void Renderer2D::Init()
	{
		GG_PROFILE_FUNCTION();

		s_Data = new Renderer2DData();

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

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data->MaxIndices);
		delete[] quadIndices;

		s_Data->QuadVertexArray = VertexArray::Create();
		s_Data->QuadVertexBuffer = VertexBuffer::Create(s_Data->MaxVertices * sizeof(QuadVertex));
		s_Data->QuadVertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_position" },
				{ ShaderDataType::Float4, "a_color"},
				{ ShaderDataType::Float2, "a_texCords" },
				{ ShaderDataType::Float, "a_texIndex" },
				{ ShaderDataType::Float2, "a_tiling" },
				{ ShaderDataType::Int, "a_entityId" }
			});

		s_Data->QuadVertexArray->AddVertexBuffer(s_Data->QuadVertexBuffer);
		s_Data->QuadVertexArray->AddIndexBuffer(quadIndexBuffer);
		s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];

		// Circles
		s_Data->CircleVertexArray = VertexArray::Create();
		s_Data->CircleVertexBuffer = VertexBuffer::Create(s_Data->MaxVertices * sizeof(CircleVertex));
		s_Data->CircleVertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_worldPosition" },
				{ ShaderDataType::Float3, "a_localPosition" },
				{ ShaderDataType::Float4, "a_color" },
				{ ShaderDataType::Float, "a_thickness" },
				{ ShaderDataType::Float, "a_fade" },
				{ ShaderDataType::Int, "a_entityId" }
			});

		s_Data->CircleVertexArray->AddVertexBuffer(s_Data->CircleVertexBuffer);
		s_Data->CircleVertexArray->AddIndexBuffer(quadIndexBuffer); // Use quad ib
		s_Data->CircleVertexBufferBase = new CircleVertex[s_Data->MaxVertices];

		// Lines
		s_Data->LineVertexArray = VertexArray::Create();
		s_Data->LineVertexBuffer = VertexBuffer::Create(s_Data->MaxVertices * sizeof(LineVertex));
		s_Data->LineVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_position" },
				{ ShaderDataType::Float4, "a_color"},
				{ ShaderDataType::Int, "a_entityId" }
			});

		s_Data->LineVertexArray->AddVertexBuffer(s_Data->LineVertexBuffer);
		s_Data->LineVertexBufferBase = new LineVertex[s_Data->MaxVertices];

		// White texture
		uint32_t whiteData = 0xffffffff;
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		s_Data->WhiteTexture->SetData(&whiteData, sizeof(uint32_t));
		s_Data->TextureSlots[0] = s_Data->WhiteTexture;

		// Shaders
		s_Data->SpriteShader = Shader::Create("assets/shaders/Renderer2D_SpriteShader.glsl");
		s_Data->CircleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");
		s_Data->LineShader = Shader::Create("assets/shaders/Renderer2D_Line.glsl");
		
		int32_t samplers[s_Data->MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data->MaxTextureSlots; ++i)
			samplers[i] = i;

		s_Data->SpriteShader->Bind();
		s_Data->SpriteShader->SetIntArray("u_textures", samplers, s_Data->MaxTextureSlots);

		s_Data->CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
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

		s_Data->CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data->CameraUniformBuffer->SetData(&s_Data->CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		GG_PROFILE_FUNCTION();

		s_Data->CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data->CameraUniformBuffer->SetData(&s_Data->CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		GG_PROFILE_FUNCTION();

		s_Data->CameraBuffer.ViewProjection = camera.GetViewPorjection();
		s_Data->CameraUniformBuffer->SetData(&s_Data->CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		GG_PROFILE_FUNCTION();
		Flush();
	}

	void Renderer2D::Flush()
	{
		GG_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase);
			s_Data->QuadVertexBuffer->SetData(s_Data->QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data->TextureSlotIndex; ++i)
				s_Data->TextureSlots[i]->Bind(i);

			s_Data->SpriteShader->Bind();
			RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
			s_Data->Stats.DrawCalls++;
		}

		if (s_Data->CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data->CircleVertexBufferPtr - (uint8_t*)s_Data->CircleVertexBufferBase);
			s_Data->CircleVertexBuffer->SetData(s_Data->CircleVertexBufferBase, dataSize);

			s_Data->CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data->CircleVertexArray, s_Data->CircleIndexCount);
			s_Data->Stats.DrawCalls++;
		}

		if (s_Data->LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data->LineVertexBufferPtr - (uint8_t*)s_Data->LineVertexBufferBase);
			s_Data->LineVertexBuffer->SetData(s_Data->LineVertexBufferBase, dataSize);

			s_Data->LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data->LineWidth);
			RenderCommand::DrawLines(s_Data->LineVertexArray, s_Data->LineVertexCount);
			s_Data->Stats.DrawCalls++;
		}
	}
	
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
		transform = glm::rotate(transform, angle, { 0.0f, 0.0f, -1.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });
		
		DrawQuad(transform, color, texture, tiling);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = GetTextureIndex(texture);

		for (uint32_t i = 0; i < 4; ++i)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = color;
			s_Data->QuadVertexBufferPtr->TexCoord = s_Data->QuadVertexUv[i];
			s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
			s_Data->QuadVertexBufferPtr->Tiling = tiling;
			s_Data->QuadVertexBufferPtr->EntityId = -1;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;
		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec2& tiling)
	{
		DrawQuad(position, size, glm::vec4(1.0f), subTexture, tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<SubTexture2D>& subTexture, const glm::vec2& tiling)
	{
		DrawQuad(position, size, angle, glm::vec4(1.0f), subTexture, tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		DrawQuad(transform, color, subTexture, tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, const glm::vec2& tiling)
	{
		GG_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, angle, { 0.0f, 0.0f, -1.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		DrawQuad(transform, color, subTexture, tiling);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId)
	{
		GG_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = GetTextureIndex(s_Data->WhiteTexture);

		for (uint32_t i = 0; i < 4; ++i)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = color;
			s_Data->QuadVertexBufferPtr->TexCoord = s_Data->QuadVertexUv[i];
			s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
			s_Data->QuadVertexBufferPtr->Tiling = { 1.0f, 1.0f };
			s_Data->QuadVertexBufferPtr->EntityId = entityId;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;
		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling, int entityId)
	{
		GG_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = GetTextureIndex(texture);

		for (uint32_t i = 0; i < 4; ++i)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = color;
			s_Data->QuadVertexBufferPtr->TexCoord = s_Data->QuadVertexUv[i];
			s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
			s_Data->QuadVertexBufferPtr->Tiling = tiling;
			s_Data->QuadVertexBufferPtr->EntityId = entityId;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;
		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, const glm::vec2& tiling, int entityId)
	{
		GG_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = GetTextureIndex(subTexture->GetTexture());
		const glm::vec2* texCoords = subTexture->GetTexCoords();

		for (uint32_t i = 0; i < 4; ++i)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = color;
			s_Data->QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data->QuadVertexBufferPtr->TextureId = textureIndex;
			s_Data->QuadVertexBufferPtr->Tiling = tiling;
			s_Data->QuadVertexBufferPtr->EntityId = entityId;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;
		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityId)
	{
		GG_PROFILE_FUNCTION();

		// TODO: Implement for circles
		//if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
		//{
		//	NextBatch();
		//}

		for (uint32_t i = 0; i < 4; ++i)
		{
			s_Data->CircleVertexBufferPtr->WorldPosition = transform * s_Data->QuadVertexPositions[i];
			s_Data->CircleVertexBufferPtr->LocalPosition = s_Data->QuadVertexPositions[i] * 2.0f;
			s_Data->CircleVertexBufferPtr->Color = color;
			s_Data->CircleVertexBufferPtr->Thickness = thickness;
			s_Data->CircleVertexBufferPtr->Fade = fade;
			s_Data->CircleVertexBufferPtr->EntityId = entityId;
			s_Data->CircleVertexBufferPtr++;
		}

		s_Data->CircleIndexCount += 6;
		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityId)
	{
		s_Data->LineVertexBufferPtr->Position = p0;
		s_Data->LineVertexBufferPtr->Color = color;
		s_Data->LineVertexBufferPtr->EntityId = entityId;
		s_Data->LineVertexBufferPtr++;

		s_Data->LineVertexBufferPtr->Position = p1;
		s_Data->LineVertexBufferPtr->Color = color;
		s_Data->LineVertexBufferPtr->EntityId = entityId;
		s_Data->LineVertexBufferPtr++;

		s_Data->LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2 size, const glm::vec4& color, int entityId)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityId)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
		{
			lineVertices[i] = transform * s_Data->QuadVertexPositions[i];
		}

		DrawLine(lineVertices[0], lineVertices[1], color);
		DrawLine(lineVertices[1], lineVertices[2], color);
		DrawLine(lineVertices[2], lineVertices[3], color);
		DrawLine(lineVertices[3], lineVertices[0], color);
	}
	
	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityId)
	{
		GG_PROFILE_FUNCTION();

		if (src.Sprite)
		{
			DrawQuad(transform, src.Color, src.Sprite, src.TilingFactor, entityId);
		}
		else
		{
			DrawQuad(transform, src.Color, entityId);
		}
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data->LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data->LineWidth = width;
	}

	Renderer2D::Statistics Renderer2D::GetStatistics()
	{
		return s_Data->Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data->Stats, 0, sizeof(Statistics));
	}

	void Renderer2D::StartBatch()
	{
		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;

		s_Data->CircleIndexCount = 0;
		s_Data->CircleVertexBufferPtr = s_Data->CircleVertexBufferBase;

		s_Data->LineVertexCount = 0;
		s_Data->LineVertexBufferPtr = s_Data->LineVertexBufferBase;

		s_Data->TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
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
			if (s_Data->TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			{
				NextBatch();
			}

			textureIndex = (float)s_Data->TextureSlotIndex;
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
			s_Data->TextureSlotIndex++;
		}

		return textureIndex;
	}
}
