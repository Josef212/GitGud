#pragma once

namespace GitGud
{
	class EditorPanel
	{
	public:
		EditorPanel() = default;	
		virtual ~EditorPanel() = default;

		virtual void OnAttach() {}
		virtual void OnDettach() {}

		virtual void OnImGui() = 0;
	};
}