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

	_checkerTexture = GitGud::Texture2D::Create("assets/textures/Checkerboard.png");
	_logoTexture = GitGud::Texture2D::Create("assets/textures/GitGudIconLogo.png");
}

void Sandbox2D::OnDetach()
{
	GG_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(GitGud::Timestep ts)
{
	GG_PROFILE_FUNCTION();

	_cameraController.OnUpdate(ts);

	GitGud::Renderer2D::ResetStats();

	GitGud::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	GitGud::RenderCommand::Clear();
	
	GitGud::Renderer2D::BeginScene(_cameraController.GetCamera());

	static float rot = 0.0f;
	rot += ts * 20.0f;

	GitGud::Renderer2D::DrawQuad({ _pos.x, _pos.y, 0.0f }, _size, _angle, _color);
	GitGud::Renderer2D::DrawQuad({ -1.0f, 2.0f, 0.0f }, { 1.0f, 1.0f }, rot, {0.2f, 0.3f, 0.8f, 1.0f});
	GitGud::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, 0.0f, _checkerTexture, glm::vec2(10.0f, 10.0f));
	GitGud::Renderer2D::DrawQuad({ 4.0f, -4.0f, 0.0f }, {1.0f, 1.0f}, 0.0f, _logoTexture);
	GitGud::Renderer2D::DrawQuad({ 1.2f, 1.4f, 0.0f }, { 1.0f, 1.0f }, 45.0f, { 0.2f, 0.8f, 0.4f, 1.0f }, _checkerTexture, glm::vec2(1.0f));

	GitGud::Renderer2D::EndScene();
	
	GitGud::Renderer2D::BeginScene(_cameraController.GetCamera());

	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
			GitGud::Renderer2D::DrawQuad({ x, y, 0.0f }, { 0.45f, 0.45f }, color);
		}
	}

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

	auto stats = GitGud::Renderer2D::GetStatistics();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indicies: %d", stats.GetTotalIndexCount());

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
