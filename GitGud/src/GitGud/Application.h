#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace GitGud
{
	class GG_API Application
	{
	public:
		Application();
		~Application();

		void Run();

	private:
		std::unique_ptr<Window> _window;
		bool _running = true;
	};

	Application* CreateApplication();
}
