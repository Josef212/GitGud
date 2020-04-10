#include <GitGud.h>

#include <imgui/imgui.h>

class ExampleLayer : public GitGud::Layer
{
public:
	ExampleLayer() : Layer("Example"), _camera(-1.6f, 1.6f, -0.9f, 0.9f), _cameraPos(0.0f)
	{
		// ----------------------------
		// Triangle
		{
			float vertices[3 * 7] =
			{
				-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.2f, 1.0f,
				 0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.2f, 1.0f,
				 0.0f,  0.5f, 0.0f,		0.2f, 0.2f, 0.8f, 1.0f
			};

			uint indices[] =
			{
				0, 1, 2
			};

			_triVertexArray.reset(GitGud::VertexArray::Create());

			std::shared_ptr<GitGud::IndexBuffer> indexBuffer;
			indexBuffer.reset(GitGud::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint)));

			std::shared_ptr<GitGud::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(GitGud::VertexBuffer::Create(vertices, sizeof(vertices)));

			GitGud::BufferLayout layout =
			{
				{GitGud::ShaderDataType::Float3, "a_position"},
				{GitGud::ShaderDataType::Float4, "a_color"}
			};

			vertexBuffer->SetLayout(layout);
			_triVertexArray->AddVertexBuffer(vertexBuffer);
			_triVertexArray->AddIndexBuffer(indexBuffer);
		}

		// -----------
		// Quad
		{
			float vertices[4 * 7] =
			{
				-0.75f, -0.75f, 0.0f,		0.2f, 0.2f, 0.8f, 1.0f,
				 0.75f, -0.75f, 0.0f,		0.2f, 0.2f, 0.8f, 1.0f,
				 0.75f,  0.75f, 0.0f,		0.2f, 0.2f, 0.8f, 1.0f,
				-0.75f,  0.75f, 0.0f,		0.2f, 0.2f, 0.8f, 1.0f
			};

			uint indices[] =
			{
				0, 1, 2, 0, 2, 3
			};

			_quadVertexArray.reset(GitGud::VertexArray::Create());

			std::shared_ptr<GitGud::IndexBuffer> indexBuffer;
			indexBuffer.reset(GitGud::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint)));

			std::shared_ptr<GitGud::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(GitGud::VertexBuffer::Create(vertices, sizeof(vertices)));

			GitGud::BufferLayout layout =
			{
				{GitGud::ShaderDataType::Float3, "a_position"},
				{GitGud::ShaderDataType::Float4, "a_color"}
			};

			vertexBuffer->SetLayout(layout);
			_quadVertexArray->AddVertexBuffer(vertexBuffer);
			_quadVertexArray->AddIndexBuffer(indexBuffer);
		}

		// -----------

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_vp;

			out vec4 v_color;

			void main()
			{
				v_color = a_color;
				gl_Position = u_vp * vec4(a_position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec4 v_color;

			void main()
			{
				color = v_color;
			}
		)";

		_shader.reset(new GitGud::Shader(vertexSrc, fragmentSrc));

		_camera.SetPosition({ 0.3f, 0.3f, 0.0f });
		_camera.SetRotation(-30.0f);
	}

	virtual void OnUpdate(GitGud::Timestep ts) override
	{

		GG_TRACE("DT: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

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

		GitGud::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		GitGud::RenderCommand::Clear();

		GitGud::Renderer::BeginScene(_camera);

		GitGud::Renderer::Submit(_shader, _quadVertexArray);
		GitGud::Renderer::Submit(_shader, _triVertexArray);

		GitGud::Renderer::EndScene();
	}

	virtual void OnEvent(GitGud::Event& event) override
	{

	}

	virtual void OnImGuiRender() override
	{
		
	}

private:

private:
	std::shared_ptr<GitGud::Shader> _shader;
	std::shared_ptr<GitGud::VertexArray> _triVertexArray;
	std::shared_ptr<GitGud::VertexArray> _quadVertexArray;
	GitGud::OrthographicCamera _camera;
	
	glm::vec3 _cameraPos;
	float _cameraRotation = 0.0f;

	float _cameraMoveSpeed = 3.0f;
	float _cameraRotSpeed = 45.0f;
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
