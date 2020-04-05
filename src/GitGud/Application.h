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

		inline Window& GetWindow() { return *_window; }
		inline static Application& Get() { return *s_instance; }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> _window;
		bool _running = true;
		LayerStack _layerStack;

	private:
		static Application* s_instance;
	};

	Application* CreateApplication();
}
