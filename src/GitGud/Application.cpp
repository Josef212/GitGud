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

		float vertices[3 * 3] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		uint indices[] =
		{
			0, 1, 2
		};


		_indexBuffer.reset(IndexBuffer::Create(indices, 3));

		glGenVertexArrays(1, &_vertexArray);
		glBindVertexArray(_vertexArray);

		_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0);
		
		glBindVertexArray(0);

		// -----------

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;

			out vec3 v_pos;

			void main()			
			{
				v_pos = a_position;
				gl_Position = vec4(a_position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_pos;

			void main()			
			{
				vec4 col = vec4(0.8, 0.2, 0.3, 1.0);
				col = vec4(v_pos * 0.5 + 0.5, 1.0);
				color = col;
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
			glBindVertexArray(_vertexArray);
			_indexBuffer->Bind();
			glDrawElements(GL_TRIANGLES, _indexBuffer->GetCount(), GL_UNSIGNED_INT, NULL);

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
