#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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

	template<typename T>
	static void AddComponentEntry(const std::string& label, Entity entity)
	{
		if (ImGui::MenuItem(label.c_str()))
		{
			entity.AddComponent<T>();
			ImGui::CloseCurrentPopup();
		}
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

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				_context->CreateEntity("New Entity");
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Inspector");

		if (_selectionContext)
		{
			EntityInspector(_selectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				AddComponentEntry<SpriteRendererComponent>("SpriteRenderer", _selectionContext);
				AddComponentEntry<CameraComponent>("Camera", _selectionContext);

				ImGui::EndPopup();
			}
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
			if (_selectionContext == entity)
			{
				_selectionContext = {};
			}

			_context->DestroyEntity(entity);
		}
	}

	template<typename T>
	static void ComponentInspector(Entity entity, const std::string& label, std::function<void(T&)> drawCbk, bool forceShow = false)
	{
		if (entity.HasComponent<T>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			const auto nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
			
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), nodeFlags, label.c_str());
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+", ImVec2{ 20, 20 }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			ImGui::PopStyleVar();

			bool remove = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if(ImGui::MenuItem("Remove component"))
					remove = true;

				ImGui::EndPopup();
			}

			if (open || forceShow)
			{
				if (drawCbk != nullptr)
				{
					drawCbk(entity.GetComponent<T>());
				}

				ImGui::TreePop();
			}

			if (remove)
			{
				entity.RemoveComponent<T>();
			}
		}
	}

	static void DrawVector3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Text(label.c_str());
		
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
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

		ComponentInspector<TransformComponent>(entity, "Transform", [&](TransformComponent& transform)
			{
				glm::vec3 rot = glm::degrees(transform.Rotation);

				DrawVector3Control("Position", transform.Translation);
				DrawVector3Control("Rotation", rot);
				DrawVector3Control("Scale", transform.Scale, 1.0f);

				transform.Rotation = glm::radians(rot);
			});
		
		ComponentInspector<CameraComponent>(entity, "Camera", [&](CameraComponent& cameraCmp)
			{
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
			});

		ComponentInspector<SpriteRendererComponent>(entity, "Sprite Renderer", [&](SpriteRendererComponent& spriteRenderer)
			{
				ImGui::ColorEdit4("Tint", glm::value_ptr(spriteRenderer.Color));
			});
	}
}
