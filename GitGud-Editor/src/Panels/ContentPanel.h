#pragma once

#include "EditorPanel.h"

namespace GitGud
{
	class ContentPanel : public EditorPanel
	{
	public:
		ContentPanel() = default;
		~ContentPanel() = default;
		
		void OnImGui() override;

	private:
	};
}
