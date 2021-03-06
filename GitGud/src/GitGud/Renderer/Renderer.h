#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "Cameras/OrthographicCamera.h"

#include <glm/glm.hpp>

namespace GitGud
{	
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transfrom = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectMatrix;
		};

		static SceneData* _sceneData;
	};
}
