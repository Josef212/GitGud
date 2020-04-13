#include "ggpch.h"
#include "Application.h"

#include "GitGud/Renderer/Renderer.h"

#include <GLFW/glfw3.h> // TMP

namespace GitGud
{
#define BIND_APPLICATION_EVENT_FN(x) GG_BIND_EVENT_FN(Application::x)

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		GG_PROFILE_FUNCTION();

		GG_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		_window = Window::Create();
		_window->SetEventCallback(BIND_APPLICATION_EVENT_FN(OnEvent));

		Renderer::Init();

		_imguiLayer = new ImGuiLayer();
		PushOverlay(_imguiLayer);
	}

	Application::~Application()
	{
		GG_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::Run()
	{
		GG_PROFILE_FUNCTION();

		while (_running)
		{
			GG_PROFILE_SCOPE("MainLoop");

			float time = (float)glfwGetTime(); // TMP
			Timestep timeStep = time - _lastFrameTime;
			_lastFrameTime = time;

			if (!_minimized)
			{
				GG_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : _layerStack)
				{
					layer->OnUpdate(timeStep);
				}
			}
			
			{
				GG_PROFILE_SCOPE("LayerStack OnImGuiRender");

				_imguiLayer->Begin();
				for (Layer* layer : _layerStack)
				{
					layer->OnImGuiRender();
				}
				_imguiLayer->End();
			}

			_window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		GG_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_APPLICATION_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_APPLICATION_EVENT_FN(OnWindowResize));

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
		GG_PROFILE_FUNCTION();

		_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		GG_PROFILE_FUNCTION();

		_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_running = false;

		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		GG_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			_minimized = true;
			return false;
		}

		_minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}
