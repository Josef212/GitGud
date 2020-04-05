#pragma once

#include "Core.h"

#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"


namespace GitGud
{
	class GG_API Application
	{
	public:
		Application();
		~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		bool OnWindowClosed(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> _window;
		bool _running = true;
		LayerStack _layerStack;
	};

	Application* CreateApplication();
}
