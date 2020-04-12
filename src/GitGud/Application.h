#pragma once

#include "Core.h"

#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"

#include "Core/TimeStep.h"

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
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> _window;
		ImGuiLayer* _imguiLayer;
		bool _running = true;
		bool _minimized = false;
		LayerStack _layerStack;
		float _lastFrameTime = 0.0f;

	private:
		static Application* s_instance;
	};

	Application* CreateApplication();
}
