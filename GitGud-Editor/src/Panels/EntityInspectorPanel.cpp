#include "EntityInspectorPanel.h"

#include "../EditorSelection.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <type_traits>

namespace GitGud
{
	// TODO: Projects
	extern const std::filesystem::path g_assetsPath;

	template<typename T>
	static void AddComponentEntry(const std::string& label, Entity entity)
	{
		if (entity.HasComponent<T>())
		{
			return;
		}

		if (ImGui::MenuItem(label.c_str()))
		{
			entity.AddComponent<T>();
			ImGui::CloseCurrentPopup();
		}
	}

	template<typename T, typename UIFunction>
	static void ComponentInspector(Entity entity, const std::string& label, UIFunction drawCbk, bool forceShow = false)
	{
		if (entity.HasComponent<T>())
		{
			const auto nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::Separator();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), nodeFlags, label.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool remove = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (!std::is_same<T, TransformComponent>::value && ImGui::MenuItem("Remove component"))
					remove = true;

				ImGui::EndPopup();
			}

			if (open || forceShow)
			{
				drawCbk(entity.GetComponent<T>());
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
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

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
		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}


	void EntityInspectorPanel::OnImGui()
	{
		ImGui::Begin("Inspector");

		Entity selected = EditorSelection::GetSelection();
		if (selected)
		{
			EntityInspector(selected);
		}

		ImGui::End();
	}
	
	void EntityInspectorPanel::EntityInspector(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			Entity selected = EditorSelection::GetSelection();
			AddComponentEntry<SpriteRendererComponent>("SpriteRenderer", selected);
			AddComponentEntry<CircleRendererComponent>("CircleRenderer", selected);
			AddComponentEntry<CameraComponent>("Camera", selected);
			AddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D", selected);
			AddComponentEntry<BoxCollider2DComponent>("BoxCollider 2D", selected);
			AddComponentEntry<CircleCollider2DComponent>("CircleCollider 2D", selected);

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		ComponentInspector<TransformComponent>(entity, "Transform", [&](auto& transform)
			{
				glm::vec3 rot = glm::degrees(transform.Rotation);

				DrawVector3Control("Position", transform.Translation);
				DrawVector3Control("Rotation", rot);
				DrawVector3Control("Scale", transform.Scale, 1.0f);

				transform.Rotation = glm::radians(rot);
			});

		ComponentInspector<CameraComponent>(entity, "Camera", [&](auto& cameraCmp)
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

		ComponentInspector<SpriteRendererComponent>(entity, "Sprite Renderer", [&](auto& spriteRenderer)
			{
				ImGui::ColorEdit4("Tint", glm::value_ptr(spriteRenderer.Color));

				ImGui::Button("Sprite", ImVec2(100.f, 0.f));
				if (ImGui::BeginDragDropTarget())
				{
					if (auto payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						auto relativePath = (const wchar_t*)payload->Data;
						auto path = g_assetsPath / relativePath;

						spriteRenderer.Sprite = Texture2D::Create(path.string());
					}

					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat2("Tiling Factor", glm::value_ptr(spriteRenderer.TilingFactor), 0.1, 0.f, 100.f);
			});

		ComponentInspector<CircleRendererComponent>(entity, "Circle Renderer", [&](CircleRendererComponent& circleRenderer)
			{
				ImGui::ColorEdit4("Tint", glm::value_ptr(circleRenderer.Color));
				ImGui::DragFloat("Thickness", &circleRenderer.Thickness, 0.025, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &circleRenderer.Fade, 0.00025f, 0.0f, 1.0f);
			});

		ComponentInspector<Rigidbody2DComponent>(entity, "Rigidbody 2D", [&](auto& rb)
			{
				const char* bodyTypeStr[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyType = bodyTypeStr[(int)rb.Type];

				if (ImGui::BeginCombo("Body Type", currentBodyType))
				{
					for (int i = 0; i < 3; ++i)
					{
						bool isSelected = (int)rb.Type == i;

						if (ImGui::Selectable(bodyTypeStr[i], isSelected))
						{
							rb.Type = (Rigidbody2DComponent::BodyType)i;
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &rb.FixedRotation);
			});

		ComponentInspector<BoxCollider2DComponent>(entity, "Box Collider 2D", [&](BoxCollider2DComponent& collider)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(collider.Offset));
				ImGui::DragFloat2("Size", glm::value_ptr(collider.Size));
				ImGui::DragFloat("Density", &collider.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &collider.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &collider.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &collider.RestitutionThreshold, 0.01f, 0.0f);
			});

		ComponentInspector<CircleCollider2DComponent>(entity, "Box Collider 2D", [&](CircleCollider2DComponent& collider)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(collider.Offset));
				ImGui::DragFloat("Radius", &collider.Radius, 0.05f, 0.0f);
				ImGui::DragFloat("Density", &collider.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &collider.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &collider.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &collider.RestitutionThreshold, 0.01f, 0.0f);
			});
	}
}