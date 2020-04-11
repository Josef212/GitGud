#include "ggpch.h"
#include "Application.h"

#include <GLFW/glfw3.h> // TMP

namespace GitGud
{
#define BIND_APPLICATION_EVENT_FN(x) GG_BIND_EVENT_FN(Application::x)

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		GG_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		_window = Scope<Window>(Window::Create());
		_window->SetEventCallback(BIND_APPLICATION_EVENT_FN(OnEvent));

		_imguiLayer = new ImGuiLayer();
		PushOverlay(_imguiLayer);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (_running)
		{
			float time = (float)glfwGetTime(); // TMP
			Timestep timeStep = time - _lastFrameTime;
			_lastFrameTime = time;

			for (Layer* layer : _layerStack)
			{
				layer->OnUpdate(timeStep);
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
		dispatcher.Dispatch<WindowCloseEvent>(BIND_APPLICATION_EVENT_FN(OnWindowClosed));

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
