#pragma once

#include "EditorPanel.h"
#include "GitGud/Renderer/Texture.h"

#include <filesystem>

namespace GitGud
{
	class ContentPanel : public EditorPanel
	{
	public:
		ContentPanel();
		~ContentPanel() = default;
		
		void OnImGui() override;

	private:
		Ref<Texture2D> GetPathIcon(std::filesystem::directory_entry dir);

	private:
		std::filesystem::path _currentDirectory;

		Ref<Texture2D> _directoryIcon;
		Ref<Texture2D> _fileIcon;
	};
}
