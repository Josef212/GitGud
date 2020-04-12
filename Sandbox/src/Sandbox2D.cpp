#include "Sandbox2D.h"
#include <imgui/imgui.h>

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

	GitGud::Renderer2D::DrawQuad(_pos, _size, _angle, _color);

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
	ImGui::DragFloat2("Pos", &_pos.x, 0.1f);
	ImGui::DragFloat2("Size", &_size.x, 0.1f);
	ImGui::DragFloat("Angle", &_angle);
	
	ImGui::End();
}
