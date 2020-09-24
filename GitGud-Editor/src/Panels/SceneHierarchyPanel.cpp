#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			_selectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Inspector");

		if (_selectionContext)
		{
			EntityInspector(_selectionContext);
		}

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

	void SceneHierarchyPanel::EntityInspector(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Trasnform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;

				ImGui::DragFloat3("Postion", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraCmp = entity.GetComponent<CameraComponent>();
				auto& camera = cameraCmp.Camera;

				ImGui::Checkbox("Primary", &cameraCmp.Primary);

				const char* projectionTypesStr[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeStr = projectionTypesStr[(int)camera.GetProjectionType()];
				
				if (ImGui::BeginCombo("Projection", currentProjectionTypeStr))
				{
					for (int i = 0; i < 2; ++i)
					{
						bool isSelected = (int)camera.GetProjectionType() == i;
						
						if (ImGui::Selectable(projectionTypesStr[i], isSelected))
						{
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveFov = glm::degrees(camera.GetPerspectiveVerticalFov());
					float perspectiveNear = camera.GetPerspectiveNearClip();
					float perspectiveFar = camera.GetPerspectiveFarClip();

					if (ImGui::DragFloat("Fov", &perspectiveFov, 0.1f))
						camera.SetPerspectiveVerticalFov(glm::radians(perspectiveFov));

					if (ImGui::DragFloat("Near", &perspectiveNear, 0.1f))
						camera.SetPerspectiveNearClip(perspectiveNear);

					if (ImGui::DragFloat("Far", &perspectiveFar, 0.1f))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					float orthoNear = camera.GetOrthographicNearClip();
					float orthoFar = camera.GetOrthographicFarClip();

					if (ImGui::DragFloat("Size", &orthoSize, 0.1f))
						camera.SetOrthographicSize(orthoSize);

					if (ImGui::DragFloat("Near", &orthoNear, 0.1f))
						camera.SetOrthographicNearClip(orthoNear);

					if (ImGui::DragFloat("Far", &orthoFar, 0.1f))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraCmp.FixedAsectRatio);
				}

				ImGui::TreePop();
			}
		}
	}
}
