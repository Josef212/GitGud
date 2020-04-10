#include "ggpch.h"
#include "Application.h"

#include "Input.h"

#include <glad/glad.h>

namespace GitGud
{
#define BIND_EVENT_FN(x) GG_BIND_EVENT_FN(Application::x)

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		GG_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		_window = std::unique_ptr<Window>(Window::Create());
		_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		_imguiLayer = new ImGuiLayer();
		PushOverlay(_imguiLayer);

		// ----------------------------

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

			_triVertexArray.reset(VertexArray::Create());

			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint)));

			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

			BufferLayout layout =
			{
				{ShaderDataType::Float3, "a_position"},
				{ShaderDataType::Float4, "a_color"}
			};

			vertexBuffer->SetLayout(layout);
			_triVertexArray->AddVertexBuffer(vertexBuffer);
			_triVertexArray->AddIndexBuffer(indexBuffer);
		}

		// -----------

		{
			float vertices[4 * 7] =
			{
				-0.5f, -0.5f, 0.0f,		0.2f, 0.2f, 0.8f, 1.0f,
				 0.5f, -0.5f, 0.0f,		0.2f, 0.2f, 0.8f, 1.0f,
				 0.5f,  0.5f, 0.0f,		0.2f, 0.2f, 0.8f, 1.0f,
				-0.5f,  0.5f, 0.0f,		0.2f, 0.2f, 0.8f, 1.0f
			};

			uint indices[] =
			{
				0, 1, 2, 0, 2, 3
			};

			_quadVertexArray.reset(VertexArray::Create());

			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint)));

			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

			BufferLayout layout =
			{
				{ShaderDataType::Float3, "a_position"},
				{ShaderDataType::Float4, "a_color"}
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

			out vec4 v_color;

			void main()
			{
				v_color = a_color;
				gl_Position = vec4(a_position, 1.0);
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

		_shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (_running)
		{

			// ----------------------------------------------------

			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			_shader->Bind();

			_quadVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, _quadVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, NULL);

			_triVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, _triVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, NULL);

			// ----------------------------------------------------

			for (Layer* layer : _layerStack)
			{
				layer->OnUpdate();
			}

			_imguiLayer->Begin();
			for (Layer* layer : _layerStack)
			{
				layer->OnImGuiRender();
			}
			_imguiLayer->End();

			_window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

		//GG_CORE_TRACE("{0}", e);

		for (auto it = _layerStack.end(); it != _layerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
		layer->OnAtach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		_layerStack.PushOverlay(overlay);
		overlay->OnAtach();
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		_running = false;

		return true;
	}
}
