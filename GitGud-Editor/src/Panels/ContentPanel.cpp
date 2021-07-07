#include "ContentPanel.h"

#include <imgui/imgui.h>

namespace GitGud
{
	static std::filesystem::path s_assetsPath = "assets";

	ContentPanel::ContentPanel() : _currentDirectory(s_assetsPath)
	{ 
		// TODO: Manage editor resources properly
		_directoryIcon = Texture2D::Create("EditorAssets/Icons/DirectoryIcon.png");
		_fileIcon = Texture2D::Create("EditorAssets/Icons/FileIcon.png");
	}

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

		static float iconSize = 64.f;
		static float padding = 12.f;
		auto with = ImGui::GetContentRegionAvailWidth();

		float size = iconSize + padding;
		auto cellCount = (int)(with / size);
		cellCount = cellCount < 1 ? 1 : cellCount;

		ImGui::Columns(cellCount, "", false);

		for (auto& dir : std::filesystem::directory_iterator(_currentDirectory))
		{
			const auto path = dir.path();
			auto relativePath = std::filesystem::relative(path, s_assetsPath);
			auto pathStr = relativePath.filename().string();
			auto icon = GetPathIcon(dir);

			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { iconSize, iconSize }, { 0, 1 }, { 1, 0 });
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (dir.is_directory())
				{
					_currentDirectory /= path.filename();
				}
			}

			ImGui::TextWrapped("%s", pathStr.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("IconSize", &iconSize, 16.f, 512.f);
		ImGui::SliderFloat("Padding", &padding, 0.f, 64.f);

		ImGui::End();
	}

	Ref<Texture2D> ContentPanel::GetPathIcon(std::filesystem::directory_entry dir)
	{
		if (dir.is_directory())
		{
			return _directoryIcon;
		}

		// TODO: Add more icons for each file type
		return _fileIcon;
	}
}
