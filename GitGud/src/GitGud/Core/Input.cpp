#include "ggpch.h"
#include "Input.h"

#ifdef GG_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace GitGud
{
	Scope<Input> Input::s_instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef GG_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		GG_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}