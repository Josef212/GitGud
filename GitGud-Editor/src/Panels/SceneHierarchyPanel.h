#pragma once

#include "GitGud.h"

namespace GitGud
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);
		~SceneHierarchyPanel();

		void SetContext(const Ref<Scene>& scene);

		void OnImGui();

		Entity GetSelectedEntity() const { return _selectionContext; }

	private:
		void EntityNode(Entity entity);
		void EntityInspector(Entity entity);

	private:
		Ref<Scene> _context;
		Entity _selectionContext;

	};
}
