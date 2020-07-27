#pragma once

#include "Cameras/OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace GitGud
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tiling = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec2& tiling = { 1.0f, 1.0f });

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling = { 1.0f, 1.0f });

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tiling);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec2& tiling = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<SubTexture2D>& subTexture, const glm::vec2& tiling = { 1.0f, 1.0f });

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, const glm::vec2& tiling = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, const glm::vec2& tiling = { 1.0f, 1.0f });

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, const glm::vec2& tiling);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static Statistics GetStatistics();
		static void ResetStats();

	private:
		static void FlushAndReset();
		static float GetTextureIndex(const Ref<Texture2D>& texture);
	};
}