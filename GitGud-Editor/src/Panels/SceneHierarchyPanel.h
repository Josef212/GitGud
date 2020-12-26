#pragma once

#include "EditorPanel.h"
#include "GitGud.h"

namespace GitGud
{
	class SceneHierarchyPanel : public EditorPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);
		~SceneHierarchyPanel();

		void SetContext(const Ref<Scene>& scene);
		void OnImGui() override;

	private:
		void EntityNode(Entity entity);

	private:
		Ref<Scene> _context;

	};
}
