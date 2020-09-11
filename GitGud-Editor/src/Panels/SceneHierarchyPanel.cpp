#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

namespace GitGud
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	SceneHierarchyPanel::~SceneHierarchyPanel()
	{
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		_context = scene;
	}

	void SceneHierarchyPanel::OnImGui()
	{
		ImGui::Begin("Scene Hierarchy");

		_context->_registry.each([&](auto entityId)
		{
			Entity entity{ entityId , _context.get() };
			EntityNode(entity);
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::EntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>();
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | (_selectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0);
		bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			_selectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}
}
