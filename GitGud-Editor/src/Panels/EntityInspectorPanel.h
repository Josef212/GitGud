#pragma once

#include "EditorPanel.h"
#include "GitGud.h"

namespace GitGud
{
	class EntityInspectorPanel : public EditorPanel
	{
	public:
		EntityInspectorPanel() = default;
		~EntityInspectorPanel() = default;

		void OnImGui() override;

	private:
		void EntityInspector(Entity entity);
	};
}
