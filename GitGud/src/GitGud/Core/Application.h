#pragma once

#include "Core.h"

#include "Window.h"
#include "GitGud/Events/ApplicationEvent.h"
#include "LayerStack.h"

#include "TimeStep.h"

#include "GitGud/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace GitGud
{
	class Application
	{
	public:
		Application();
		~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *_window; }
		inline static Application& Get() { return *s_instance; }

	private:
		void Run();
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
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();
}
