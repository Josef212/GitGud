#include "Sandbox2D.h"
#include <imgui/imgui.h>

Sandbox2D::Sandbox2D() : GitGud::Layer("2D Sandbos"), _cameraController(1280.0f / 720.0f)
{
	GG_PROFILE_FUNCTION();
}

Sandbox2D::~Sandbox2D()
{
	GG_PROFILE_FUNCTION();
}

void Sandbox2D::OnAttach()
{
	GG_PROFILE_FUNCTION();

	_texture = GitGud::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	GG_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(GitGud::Timestep ts)
{
	GG_PROFILE_FUNCTION();

	_cameraController.OnUpdate(ts);

	GitGud::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	GitGud::RenderCommand::Clear();

	GitGud::Renderer2D::BeginScene(_cameraController.GetCamera());

	GitGud::Renderer2D::DrawQuad(_pos, _size, _angle, _color);
	GitGud::Renderer2D::DrawQuad({ -1.0f, 2.0f }, { 1.0f, 1.0f }, -30.0f, {0.2f, 0.3f, 0.8f, 1.0f});
	GitGud::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, {10.0f, 10.0f}, 0.0f, _texture, glm::vec2(10.0f, 10.0f));
	GitGud::Renderer2D::DrawQuad({ 1.2f, 1.4f, 0.0f }, { 1.0f, 1.0f }, 45.0f, { 0.2f, 0.8f, 0.4f, 1.0f }, _texture, glm::vec2(1.0f));

	GitGud::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(GitGud::Event& e)
{
	GG_PROFILE_FUNCTION();

	_cameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	GG_PROFILE_FUNCTION();

	_cameraController.OnImGuiRender();

	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Color", &_color.r);
	ImGui::DragFloat2("Pos", &_pos.x, 0.1f);
	ImGui::DragFloat2("Size", &_size.x, 0.1f);
	ImGui::DragFloat("Angle", &_angle);

	if (ImGui::Button("Begin session"))
	{
		GG_PROFILE_BEGIN_SESSION("TestSession", "testSession.json");
	}

	ImGui::SameLine();

	if (ImGui::Button("End session"))
	{
		GG_PROFILE_END_SESSION();
	}
	
	ImGui::End();
}
