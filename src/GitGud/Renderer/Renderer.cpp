#include "ggpch.h"
#include "Renderer.h"

namespace GitGud
{
	Renderer::SceneData* Renderer::_sceneData = new Renderer::SceneData();
	
	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		_sceneData->ViewProjectMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transfrom)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_vp", _sceneData->ViewProjectMatrix);
		shader->UploadUniformMat4("u_model", transfrom);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}