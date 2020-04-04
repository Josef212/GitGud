#pragma once

#include "ggpch.h"

#include "GitGud/Core.h"
#include "GitGud/Events/Event.h"

namespace GitGud
{
	struct WindowProps
	{
		std::string Title;
		uint Width;
		uint Height;

		WindowProps(const std::string& title = "GitGud", uint width = 1280, uint height = 720) : Title(title), Width(width), Height(height)
		{ }
	};

	class GG_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint GetWidth() const = 0;
		virtual uint GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
