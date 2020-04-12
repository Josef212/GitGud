#include <GitGud.h>

#include "Platform/OpenGL/OpenGLShader.h" // TMP

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public GitGud::Layer
{
public:
	ExampleLayer() : Layer("Example"), _cameraController(1280.0f / 720.0f), _trianglePos(0.0f), _triangleEuler(0.0f), _triangleScale(1.0f)
	{
		// -----------

		// Triangle
		{
			float vertices[3 * 7] =
			{
				-0.5f, -0.5f, 0.0f,		0.8f, 0.1f, 0.1f, 1.0f,
				 0.5f, -0.5f, 0.0f,		0.1f, 0.1f, 0.8f, 1.0f,
				 0.0f,  0.5f, 0.0f,		0.1f, 0.8f, 0.1f, 1.0f
			};

			uint indices[] =
			{
				0, 1, 2
			};

			_triangleVA = GitGud::VertexArray::Create();

			GitGud::Ref<GitGud::IndexBuffer> indexBuffer = GitGud::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));
			GitGud::Ref<GitGud::VertexBuffer> vertexBuffer = GitGud::VertexBuffer::Create(vertices, sizeof(vertices));

			GitGud::BufferLayout layout =
			{
				{GitGud::ShaderDataType::Float3, "a_position"},
				{GitGud::ShaderDataType::Float4, "a_color"}
			};

			vertexBuffer->SetLayout(layout);
			_triangleVA->AddVertexBuffer(vertexBuffer);
			_triangleVA->AddIndexBuffer(indexBuffer);
		}

		// -----------

		// TileQuad
		{
			float vertices[4 * 7] =
			{
				-0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,
				 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,
				 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f
			};

			uint indices[] =
			{
				0, 1, 2, 0, 2, 3
			};

			_tileVA = GitGud::VertexArray::Create();

			GitGud::Ref<GitGud::IndexBuffer> indexBuffer = GitGud::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));
			GitGud::Ref<GitGud::VertexBuffer> vertexBuffer = GitGud::VertexBuffer::Create(vertices, sizeof(vertices));

			GitGud::BufferLayout layout =
			{
				{GitGud::ShaderDataType::Float3, "a_position"},
				{GitGud::ShaderDataType::Float4, "a_color"}
			};

			vertexBuffer->SetLayout(layout);
			_tileVA->AddVertexBuffer(vertexBuffer);
			_tileVA->AddIndexBuffer(indexBuffer);
		}

		// -----------

		// Quad
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

			_quadVA = GitGud::VertexArray::Create();

			GitGud::Ref<GitGud::IndexBuffer> indexBuffer = GitGud::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));
			GitGud::Ref<GitGud::VertexBuffer> vertexBuffer = GitGud::VertexBuffer::Create(vertices, sizeof(vertices));

			GitGud::BufferLayout layout =
			{
				{GitGud::ShaderDataType::Float3, "a_position"},
				{GitGud::ShaderDataType::Float2, "a_texCords"}
			};

			vertexBuffer->SetLayout(layout);
			_quadVA->AddVertexBuffer(vertexBuffer);
			_quadVA->AddIndexBuffer(indexBuffer);
		}

		// -----------
		
		// Shaders
		{
			std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_vp;
			uniform mat4 u_model;

			out vec4 v_color;

			void main()
			{
				v_color = a_color;
				gl_Position = u_vp * u_model * vec4(a_position, 1.0);
			}
		)";

			std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec4 u_color;

			in vec4 v_color;

			void main()
			{
				color = u_color * v_color;
			}
		)";

			_shader = GitGud::Shader::Create("ColorShader", vertexSrc, fragmentSrc);
		}

		auto texutreShader = _shaderLib.Load("assets/shaders/Texture.glsl");
		texutreShader->Bind();
		std::dynamic_pointer_cast<GitGud::OpenGLShader>(texutreShader)->UploadUniformInt("u_texture", 0);

		// -----------
		_checkerTexture = GitGud::Texture2D::Create("assets/textures/Checkerboard.png");
		_chernoTexture = GitGud::Texture2D::Create("assets/textures/ChernoLogo.png");
	}

	virtual void OnUpdate(GitGud::Timestep ts) override
	{
		//GG_TRACE("DT: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		_cameraController.OnUpdate(ts);

		GitGud::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		GitGud::RenderCommand::Clear();

		GitGud::Renderer::BeginScene(_cameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 red(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blue(0.2f, 0.3f, 0.8f, 1.0f);

		std::dynamic_pointer_cast<GitGud::OpenGLShader>(_shader)->Bind();

		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				std::dynamic_pointer_cast<GitGud::OpenGLShader>(_shader)->UploadUniformFloat4("u_color", (x % 2 == 0) ? red : blue);

				GitGud::Renderer::Submit(_shader, _tileVA, transform);
			}
		}

		glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), _trianglePos);
		triangleTransform = glm::rotate(triangleTransform, glm::radians(_triangleEuler.x), { 1.0f, 0.0f, 0.0f });
		triangleTransform = glm::rotate(triangleTransform, glm::radians(_triangleEuler.y), { 0.0f, 1.0f, 0.0f });
		triangleTransform = glm::rotate(triangleTransform, glm::radians(_triangleEuler.z), { 0.0f, 0.0f, 1.0f });
		triangleTransform = glm::scale(triangleTransform, _triangleScale);

		std::dynamic_pointer_cast<GitGud::OpenGLShader>(_shader)->UploadUniformFloat4("u_color", {1.0f, 1.0f, 1.0f, 1.0f});
		GitGud::Renderer::Submit(_shader, _triangleVA, triangleTransform);

		auto texutreShader = _shaderLib.Get("Texture");
		texutreShader->Bind();
		std::dynamic_pointer_cast<GitGud::OpenGLShader>(texutreShader)->UploadUniformFloat4("u_color", { 1.0f, 1.0f, 1.0f, 1.0f });
		_checkerTexture->Bind();
		GitGud::Renderer::Submit(texutreShader, _quadVA);
		
		_chernoTexture->Bind();
		GitGud::Renderer::Submit(texutreShader, _quadVA);

		GitGud::Renderer::EndScene();
	}

	virtual void OnEvent(GitGud::Event& event) override
	{
		_cameraController.OnEvent(event);
	}

	virtual void OnImGuiRender() override
	{
		_cameraController.OnImGuiRender();

		ImGui::Begin("Triangle transform");

		ImGui::DragFloat3("Position", &_trianglePos.x, 0.1f);
		ImGui::DragFloat3("Rotation", &_triangleEuler.x, 0.1f);
		ImGui::DragFloat3("Scale", &_triangleScale.x, 0.1f);

		ImGui::End();
	}

private:

private:
	GitGud::Ref<GitGud::Shader> _shader;
	GitGud::Ref<GitGud::VertexArray> _triangleVA;
	GitGud::Ref<GitGud::VertexArray> _tileVA;
	GitGud::Ref<GitGud::VertexArray> _quadVA;
	GitGud::Ref<GitGud::Texture> _checkerTexture, _chernoTexture;

	GitGud::ShaderLibrary _shaderLib;

	GitGud::OrthographicCameraController _cameraController;
	
	glm::vec3 _trianglePos;
	glm::vec3 _triangleEuler;
	glm::vec3 _triangleScale;
};

class SandboxApp : public GitGud::Application
{
public:
	SandboxApp() 
	{
		PushLayer(new ExampleLayer());
	}

	~SandboxApp() {}
};

GitGud::Application* GitGud::CreateApplication()
{
	return new SandboxApp();
}
