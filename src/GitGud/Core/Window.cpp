#include "ggpch.h"
#include "GitGud/Core/Window.h"

#ifdef GG_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace GitGud
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef GG_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		GG_CORE_ASSERT(false, "Platform not supported!");
		return nullptr;
#endif
	}
}
