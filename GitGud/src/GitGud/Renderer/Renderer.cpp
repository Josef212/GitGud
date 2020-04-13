#include "ggpch.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h" // TMP

namespace GitGud
{
	Renderer::SceneData* Renderer::_sceneData = new Renderer::SceneData();
	
	void Renderer::Init()
	{
		GG_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		GG_PROFILE_FUNCTION();

		//RenderCommand::Shutdown();
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

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