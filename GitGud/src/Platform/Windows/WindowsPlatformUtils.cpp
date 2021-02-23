#include "ggpch.h"
#include "GitGud/Utils/PlatformUtils.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "GitGud/Core/Application.h"

namespace GitGud
{
	std::optional<std::string> FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currenDir[256] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetCurrentDirectoryA(256, currenDir))
			ofn.lpstrInitialDir = currenDir;
		
		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::nullopt;
	}

	std::optional<std::string> FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currenDir[256] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
		if (GetCurrentDirectoryA(256, currenDir))
			ofn.lpstrInitialDir = currenDir;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::nullopt;
	}
}