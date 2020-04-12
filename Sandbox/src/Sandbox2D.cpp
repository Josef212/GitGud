#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : GitGud::Layer("2D Sandbos"), _cameraController(1280.0f / 720.0f)
{
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(GitGud::Timestep ts)
{
	_cameraController.OnUpdate(ts);

	GitGud::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	GitGud::RenderCommand::Clear();

	GitGud::Renderer2D::BeginScene(_cameraController.GetCamera());

	GitGud::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, _color);
	
	//_shader->Bind();
	//std::dynamic_pointer_cast<GitGud::OpenGLShader>(_shader)->UploadUniformFloat4("u_color", _color);
	//GitGud::Renderer::Submit(_shader, _va);

	GitGud::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(GitGud::Event& e)
{
	_cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	_cameraController.OnImGuiRender();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", &_color.r);
	ImGui::End();
}
