#pragma once

#include "GitGud.h"

namespace GitGud
{
	class EntityInspectorPanel
	{
	public:
		EntityInspectorPanel() = default;
		~EntityInspectorPanel() = default;

		void OnImGui();

	private:
		void EntityInspector(Entity entity);
	};
}
