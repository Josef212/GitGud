#include "ggpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h" // TMP

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

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transfrom)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_vp", _sceneData->ViewProjectMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_model", transfrom);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}