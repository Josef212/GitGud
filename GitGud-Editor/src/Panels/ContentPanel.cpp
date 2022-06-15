#include "ContentPanel.h"

#include <imgui/imgui.h>

namespace GitGud
{
	// TODO: Projects
	extern const std::filesystem::path g_assetsPath = "assets";

	ContentPanel::ContentPanel() : _currentDirectory(g_assetsPath)
	{ 
		// TODO: Manage editor resources properly
		_directoryIcon = Texture2D::Create("EditorAssets/Icons/DirectoryIcon.png");
		_fileIcon = Texture2D::Create("EditorAssets/Icons/FileIcon.png");
	}

	void ContentPanel::OnImGui()
	{
		ImGui::Begin("Content");

		if (_currentDirectory != std::filesystem::path(g_assetsPath))
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
			auto relativePath = std::filesystem::relative(path, g_assetsPath);
			auto pathStr = relativePath.filename().string();
			auto icon = GetPathIcon(dir);

			ImGui::PushID(pathStr.c_str());

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { iconSize, iconSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				auto itemRelativePath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemRelativePath, (wcslen(itemRelativePath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (dir.is_directory())
				{
					_currentDirectory /= path.filename();
				}
				else
				{
					// TODO: Open scene, prefab, select asset, etc.
				}
			}

			ImGui::TextWrapped("%s", pathStr.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
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
