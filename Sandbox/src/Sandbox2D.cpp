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
	float vertices[4 * 5] =
	{
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
	};

	uint indices[] =
	{
		0, 1, 2, 0, 2, 3
	};

	_va = GitGud::VertexArray::Create();
	GitGud::Ref<GitGud::IndexBuffer> indexBuffer = GitGud::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));
	GitGud::Ref<GitGud::VertexBuffer> vertexBuffer = GitGud::VertexBuffer::Create(vertices, sizeof(vertices));

	GitGud::BufferLayout layout =
	{
		{GitGud::ShaderDataType::Float3, "a_position"},
		{GitGud::ShaderDataType::Float2, "a_texCords"}
	};

	vertexBuffer->SetLayout(layout);
	_va->AddVertexBuffer(vertexBuffer);
	_va->AddIndexBuffer(indexBuffer);

	_shader = GitGud::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(GitGud::Timestep ts)
{
	_cameraController.OnUpdate(ts);

	GitGud::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	GitGud::RenderCommand::Clear();

	GitGud::Renderer::BeginScene(_cameraController.GetCamera());
	
	_shader->Bind();
	std::dynamic_pointer_cast<GitGud::OpenGLShader>(_shader)->UploadUniformFloat4("u_color", _color);
	GitGud::Renderer::Submit(_shader, _va);

	GitGud::Renderer::EndScene();
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
