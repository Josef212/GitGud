#include <GitGud.h>

#include "Platform/OpenGL/OpenGLShader.h" // TMP

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public GitGud::Layer
{
public:
	ExampleLayer() : Layer("Example"), _camera(-1.6f, 1.6f, -0.9f, 0.9f), _cameraPos(0.0f), _trianglePos(0.0f), _triangleEuler(0.0f), _triangleScale(1.0f)
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

			_shader = GitGud::Shader::Create(vertexSrc, fragmentSrc);
		}

		_texturedShader = GitGud::Shader::Create("assets/shaders/Texture.glsl");
		_texturedShader->Bind();
		std::dynamic_pointer_cast<GitGud::OpenGLShader>(_texturedShader)->UploadUniformInt("u_texture", 0);

		// -----------
		_checkerTexture = GitGud::Texture2D::Create("assets/textures/Checkerboard.png");
		_chernoTexture = GitGud::Texture2D::Create("assets/textures/ChernoLogo.png");
	}

	virtual void OnUpdate(GitGud::Timestep ts) override
	{
		//GG_TRACE("DT: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		UpdateCamera(ts);

		GitGud::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		GitGud::RenderCommand::Clear();

		GitGud::Renderer::BeginScene(_camera);

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

		_texturedShader->Bind();
		std::dynamic_pointer_cast<GitGud::OpenGLShader>(_texturedShader)->UploadUniformFloat4("u_color", { 1.0f, 1.0f, 1.0f, 1.0f });
		_checkerTexture->Bind();
		GitGud::Renderer::Submit(_texturedShader, _quadVA);
		
		_chernoTexture->Bind();
		GitGud::Renderer::Submit(_texturedShader, _quadVA);

		GitGud::Renderer::EndScene();
	}

	virtual void OnEvent(GitGud::Event& event) override
	{

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Triangle transform");

		ImGui::DragFloat3("Position", &_trianglePos.x, 0.1f);
		ImGui::DragFloat3("Rotation", &_triangleEuler.x, 0.1f);
		ImGui::DragFloat3("Scale", &_triangleScale.x, 0.1f);

		ImGui::End();

		ImGui::Begin("Camera");

		ImGui::DragFloat3("Position", &_cameraPos.x, 0.1f);
		ImGui::DragFloat("Rotation", &_cameraRotation, 0.1f);

		if (ImGui::Button("Reset"))
		{
			_cameraPos = { 0.0f, 0.0f, 0.0f };
			_cameraRotation = 0.0f;
			_camera.SetPosition(_cameraPos);
			_camera.SetRotation(_cameraRotation);
		}

		ImGui::DragFloat("Camera move speed", &_cameraMoveSpeed, 0.5f);
		ImGui::DragFloat("Camera rotation speed", &_cameraRotSpeed, 0.5f);

		ImGui::End();
	}

private:
	void UpdateCamera(GitGud::Timestep& ts)
	{
		if (GitGud::Input::IsKey(GG_KEY_A))
			_cameraPos.x -= _cameraMoveSpeed * ts;
		if (GitGud::Input::IsKey(GG_KEY_D))
			_cameraPos.x += _cameraMoveSpeed * ts;
		if (GitGud::Input::IsKey(GG_KEY_S))
			_cameraPos.y -= _cameraMoveSpeed * ts;
		if (GitGud::Input::IsKey(GG_KEY_W))
			_cameraPos.y += _cameraMoveSpeed * ts;

		if (GitGud::Input::IsKey(GG_KEY_Q))
			_cameraRotation += _cameraRotSpeed * ts;
		if (GitGud::Input::IsKey(GG_KEY_E))
			_cameraRotation -= _cameraRotSpeed * ts;

		_camera.SetPosition(_cameraPos);
		_camera.SetRotation(_cameraRotation);
	}

private:
	GitGud::Ref<GitGud::Shader> _shader, _texturedShader;
	GitGud::Ref<GitGud::VertexArray> _triangleVA;
	GitGud::Ref<GitGud::VertexArray> _tileVA;
	GitGud::Ref<GitGud::VertexArray> _quadVA;
	GitGud::Ref<GitGud::Texture> _checkerTexture, _chernoTexture;

	GitGud::OrthographicCamera _camera;
	
	glm::vec3 _cameraPos;
	float _cameraRotation = 0.0f;

	float _cameraMoveSpeed = 3.0f;
	float _cameraRotSpeed = 45.0f;

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
