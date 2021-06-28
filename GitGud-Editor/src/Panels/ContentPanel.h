#pragma once

#include "EditorPanel.h"

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
		std::filesystem::path _currentDirectory;
	};
}
