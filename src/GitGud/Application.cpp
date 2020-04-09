#include "ggpch.h"
#include "Application.h"

#include "Input.h"

#include <glad/glad.h>

namespace GitGud
{
#define BIND_EVENT_FN(x) GG_BIND_EVENT_FN(Application::x)

	Application* Application::s_instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case GitGud::ShaderDataType::Float:		return GL_FLOAT;
		case GitGud::ShaderDataType::Float2:	return GL_FLOAT;
		case GitGud::ShaderDataType::Float3:	return GL_FLOAT;
		case GitGud::ShaderDataType::Float4:	return GL_FLOAT;
		case GitGud::ShaderDataType::Mat3:		return GL_FLOAT;
		case GitGud::ShaderDataType::Mat4:		return GL_FLOAT;
		case GitGud::ShaderDataType::Int:		return GL_INT;
		case GitGud::ShaderDataType::Int2:		return GL_INT;
		case GitGud::ShaderDataType::Int3:		return GL_INT;
		case GitGud::ShaderDataType::Int4:		return GL_INT;
		case GitGud::ShaderDataType::Bool:		return GL_BOOL;
		}
	}

	Application::Application()
	{
		GG_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		_window = std::unique_ptr<Window>(Window::Create());
		_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		_imguiLayer = new ImGuiLayer();
		PushOverlay(_imguiLayer);

		// ----------------------------

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


		_indexBuffer.reset(IndexBuffer::Create(indices, 3));

		glGenVertexArrays(1, &_vertexArray);
		glBindVertexArray(_vertexArray);

		_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout =
			{
				{ShaderDataType::Float3, "a_position"},
				{ShaderDataType::Float4, "a_color"}
			};

			_vertexBuffer->SetLayout(layout);
		}

		const auto& layout = _vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(),
				(const void*)element.Offset);
			++index;
		}
		
		glBindVertexArray(0);

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
