#include "ContentPanel.h"

#include <imgui/imgui.h>

namespace GitGud
{
	static std::filesystem::path s_assetsPath = "assets";

	ContentPanel::ContentPanel() : _currentDirectory(s_assetsPath)
	{ }

	void ContentPanel::OnImGui()
	{
		ImGui::Begin("Content");

		if (_currentDirectory != std::filesystem::path(s_assetsPath))
		{
			if (ImGui::Button("<-"))
			{
				_currentDirectory = _currentDirectory.parent_path();
			}
		}

		for (auto& dir : std::filesystem::directory_iterator(_currentDirectory))
		{
			const auto path = dir.path();
			auto relativePath = std::filesystem::relative(path, s_assetsPath);
			auto pathStr = relativePath.filename().string();

			if (dir.is_directory())
			{
				if (ImGui::Button(pathStr.c_str()))
				{
					_currentDirectory /= path.filename();
				}
			}
			else
			{
				ImGui::Text("%s", pathStr.c_str());
			}
		}

		ImGui::End();
	}
}
