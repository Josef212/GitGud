#pragma once

#include "EditorPanel.h"

namespace GitGud
{
	class StatsPanel : public EditorPanel
	{
	public:
		StatsPanel() = default;
		~StatsPanel() = default;

		void OnImGui() override;
	};
}
