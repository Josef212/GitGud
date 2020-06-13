#include <GitGud.h>
#include <GitGud/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace GitGud
{
	class GitGudEditorApp : public Application
	{
	public:
		GitGudEditorApp() : Application("GitGud-Editor")
		{
			PushLayer(new EditorLayer());
		}

		~GitGudEditorApp() {}
	};

	Application* CreateApplication()
	{
		return new GitGudEditorApp();
	}

}