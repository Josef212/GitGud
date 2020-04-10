#pragma once

#include "Core.h"

#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"

#include "GitGud/ImGui/ImGuiLayer.h"

namespace GitGud
{
	class Application
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
		ImGuiLayer* _imguiLayer;
		bool _running = true;
		LayerStack _layerStack;

	private:
		static Application* s_instance;
	};

	Application* CreateApplication();
}
