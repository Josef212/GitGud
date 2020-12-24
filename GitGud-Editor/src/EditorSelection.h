#pragma once

#include "GitGud.h"

namespace GitGud
{
	static class EditorSelection
	{
	private:
		EditorSelection() = default;
		~EditorSelection() = default;

	public:
		static void Select(Entity selection) { _selection = selection; }
		static Entity GetSelection() { return _selection; }

	private:
		static Entity _selection;
	};
}
