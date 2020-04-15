#pragma once

#include "Cameras/OrthographicCamera.h"
#include "Texture.h"

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

	private:
		static float GetTextureIndex(const Ref<Texture2D>& texture);
	};
}