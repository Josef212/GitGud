#include "SceneHierarchyPanel.h"

#include "../EditorSelection.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <type_traits>

namespace GitGud
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) : EditorPanel()
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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			EditorSelection::Select(Entity::Null());
		}

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				_context->CreateEntity("New Entity");
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::EntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>();
		
		ImGuiTreeNodeFlags flags = 
			ImGuiTreeNodeFlags_OpenOnArrow 
			| (EditorSelection::GetSelection() == entity ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			EditorSelection::Select(entity);
		}

		bool deleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				deleted = true;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (deleted)
		{
			if (EditorSelection::GetSelection() == entity)
			{
				EditorSelection::Select(Entity::Null());
			}

			_context->DestroyEntity(entity);
		}
	}
}
